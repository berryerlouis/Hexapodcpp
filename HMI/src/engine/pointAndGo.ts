import {
    BufferGeometry,
    CircleGeometry,
    DoubleSide,
    Line,
    LineBasicMaterial,
    Mesh,
    MeshBasicMaterial,
    Object3D,
    PerspectiveCamera,
    Plane,
    Raycaster,
    RingGeometry,
    Vector2,
    Vector3,
} from 'three';
import Hexapod from "../entity/hexapod.ts";
import Socket from "../communication/socket.ts";
import DriveController from "./driveController.ts";
import RangeMap from "../entity/objects/rangeMap.ts";

/**
 * Click the floor and the hexapod turns towards the spot, then walks to it.
 * Obstacles found by the range sensors are steered around, on the side that needs the smaller detour.
 * Position comes from the HMI's own foot odometry, so the arrival is approximate.
 * Any manual input, stop or disconnection cancels the trip.
 */
export default class PointAndGo extends Object3D {
    private static readonly ARRIVE_DISTANCE = 0.35;
    // Stand-off when the clicked spot is itself inside an obstacle.
    private static readonly BLOCKED_GOAL_DISTANCE = 3;
    private static readonly MAX_DISTANCE = 40;
    private static readonly TURN_ENTER_DEG = 15;
    private static readonly TURN_EXIT_DEG = 6;
    private static readonly NEAR_DISTANCE = 0.8;
    // Obstacle clearance and look-ahead, in scene units (1 = 10 cm).
    private static readonly ROBOT_RADIUS = 1.6;
    private static readonly LOOK_AHEAD = 5;
    private static readonly SAMPLE_START = 1;
    private static readonly SAMPLE_STEP = 0.5;
    private static readonly SCAN_STEP_DEG = 15;
    private static readonly SCAN_STEPS = 11;
    private static readonly SIDE_SWITCH_PENALTY = 3;
    private static readonly MIN_FREE_DISTANCE = 1.5;
    private static readonly PLAN_INTERVAL_MS = 100;
    private static readonly RELAX_INTERVAL_MS = 800;
    private static readonly SLOW_DISTANCE = 1.5;
    private static readonly TIMEOUT_MS = 120_000;
    private static readonly CLICK_MAX_MOVE_PX = 6;
    private static readonly CLICK_MAX_MS = 350;
    private static readonly FLOOR_Y = 0.03;

    private readonly hexapod: Hexapod;
    private readonly drive: DriveController;
    private readonly map: RangeMap;
    private readonly camera: PerspectiveCamera;
    private readonly canvas: HTMLCanvasElement;
    private readonly raycaster = new Raycaster();
    private readonly floor = new Plane(new Vector3(0, 1, 0), 0);
    private readonly local = new Vector3();

    private readonly ring: Mesh;
    private readonly line: Line;

    private target: Vector3 | null = null;
    private turning = false;
    // Heading offset from the straight line to the target chosen to dodge obstacles, and the side taken.
    private detourDeg = 0;
    private detourSide = 0;
    private lastRelaxAt = 0;
    private lastPlanAt = 0;
    private startedAt = 0;
    private connected = false;
    private downX = 0;
    private downY = 0;
    private downAt = 0;

    constructor(hexapod: Hexapod, drive: DriveController, map: RangeMap, camera: PerspectiveCamera, canvas: HTMLCanvasElement, socket: Socket) {
        super();
        this.hexapod = hexapod;
        this.drive = drive;
        this.map = map;
        this.camera = camera;
        this.canvas = canvas;

        const material = new MeshBasicMaterial({ color: 0x2ee6d6, transparent: true, opacity: 0.85, side: DoubleSide, depthWrite: false });
        const ringGeometry = new RingGeometry(0.26, 0.34, 48);
        ringGeometry.rotateX(-Math.PI / 2);
        const dotGeometry = new CircleGeometry(0.07, 24);
        dotGeometry.rotateX(-Math.PI / 2);
        this.ring = new Mesh(ringGeometry, material);
        this.ring.add(new Mesh(dotGeometry, material));
        this.ring.renderOrder = 5;
        this.ring.visible = false;
        this.add(this.ring);

        this.line = new Line(
            new BufferGeometry().setFromPoints([new Vector3(), new Vector3()]),
            new LineBasicMaterial({ color: 0x2ee6d6, transparent: true, opacity: 0.55, depthWrite: false }),
        );
        this.line.renderOrder = 5;
        this.line.frustumCulled = false;
        this.line.visible = false;
        this.add(this.line);

        canvas.addEventListener('pointerdown', (e) => this.onPointerDown(e));
        canvas.addEventListener('pointerup', (e) => this.onPointerUp(e));

        socket.addCallbackStarted(() => { this.connected = true; });
        socket.addCallbackStopped(() => {
            this.connected = false;
            this.cancel(false);
        });
    }

    /** Abort the trip. `stopWalking` is false when something else is taking over the bot. */
    cancel(stopWalking: boolean) {
        if (!this.target) return;
        this.target = null;
        this.ring.visible = false;
        this.line.visible = false;
        this.drive.releaseSource(DriveController.AUTO, stopWalking);
    }

    update(now: number) {
        if (!this.target) return;

        if (this.drive.hasInputOtherThan(DriveController.AUTO)) {
            this.cancel(false);
            return;
        }
        if (now - this.startedAt > PointAndGo.TIMEOUT_MS) {
            this.cancel(true);
            return;
        }

        this.hexapod.updateMatrixWorld(true);
        this.local.copy(this.target);
        this.hexapod.worldToLocal(this.local);
        const distance = Math.hypot(this.local.x, this.local.z);
        const arrive = this.map.isBlocked(this.target.x, this.target.z, 0)
            ? PointAndGo.BLOCKED_GOAL_DISTANCE
            : PointAndGo.ARRIVE_DISTANCE;
        if (distance < arrive) {
            this.cancel(true);
            return;
        }

        // Positive = target on the left (counter-clockwise turn needed).
        const bearingDeg = (Math.atan2(-this.local.x, -this.local.z) * 180) / Math.PI;
        if (now - this.lastPlanAt >= PointAndGo.PLAN_INTERVAL_MS) {
            this.lastPlanAt = now;
            if (!this.plan(bearingDeg, Math.min(PointAndGo.LOOK_AHEAD, distance - arrive), now)) {
                this.cancel(true);
                return;
            }
        }

        const errorDeg = PointAndGo.wrapDeg(bearingDeg + this.detourDeg);
        const absError = Math.abs(errorDeg);
        const mustFace = distance > PointAndGo.NEAR_DISTANCE || this.detourDeg !== 0;
        if (!this.turning && absError > PointAndGo.TURN_ENTER_DEG && mustFace) {
            this.turning = true;
        } else if (this.turning && absError < PointAndGo.TURN_EXIT_DEG) {
            this.turning = false;
        }

        // The gait cannot walk and turn at once, so the trip alternates between the two.
        if (this.turning) {
            this.drive.setMove(DriveController.AUTO, 0, 0);
            this.drive.setTurn(DriveController.AUTO, -Math.sign(errorDeg) * Math.min(1, Math.max(0.5, absError / 45)));
        } else {
            const speed = Math.min(1, Math.max(0.4, distance / PointAndGo.SLOW_DISTANCE));
            this.drive.setMove(DriveController.AUTO, 0, speed);
            this.drive.setTurn(DriveController.AUTO, 0);
        }

        this.updateMarker(now);
    }

    private static wrapDeg(deg: number): number {
        return ((((deg + 180) % 360) + 360) % 360) - 180;
    }

    /**
     * Picks the heading offset (from the bearing of the target) closest to the straight line whose
     * look-ahead is free of obstacles. Returns false when the bot is boxed in.
     */
    private plan(bearingDeg: number, reach: number, now: number): boolean {
        if (this.freeDistance(bearingDeg + this.detourDeg, reach) >= reach) {
            // Return to the straight line one step at a time, otherwise the bot flip-flops at corners.
            if (this.detourDeg !== 0 && now - this.lastRelaxAt >= PointAndGo.RELAX_INTERVAL_MS) {
                const relaxed = this.detourDeg - Math.sign(this.detourDeg) * PointAndGo.SCAN_STEP_DEG;
                if (this.freeDistance(bearingDeg + relaxed, reach) >= reach) {
                    this.detourDeg = relaxed;
                    this.detourSide = Math.sign(relaxed);
                    this.lastRelaxAt = now;
                }
            }
            return true;
        }

        let bestCost = Infinity;
        let bestOffset = 0;
        let farthest = -1;
        let farthestOffset = 0;

        for (let k = 0; k <= PointAndGo.SCAN_STEPS; k++) {
            for (const sign of k === 0 ? [1] : [1, -1]) {
                const offset = sign * k * PointAndGo.SCAN_STEP_DEG;
                const free = this.freeDistance(bearingDeg + offset, reach);
                if (free > farthest) {
                    farthest = free;
                    farthestOffset = offset;
                }
                if (free < reach) continue;
                const switchesSide = k > 0 && this.detourSide !== 0 && sign !== this.detourSide;
                const cost = k + (switchesSide ? PointAndGo.SIDE_SWITCH_PENALTY : 0);
                if (cost < bestCost) {
                    bestCost = cost;
                    bestOffset = offset;
                }
            }
        }

        if (bestCost === Infinity) {
            if (farthest < PointAndGo.MIN_FREE_DISTANCE) return false;
            bestOffset = farthestOffset;
        }
        this.detourDeg = bestOffset;
        this.detourSide = Math.sign(bestOffset);
        return true;
    }

    // Distance along a heading (relative to the bot) before the first obstacle, capped at `reach`.
    private freeDistance(relativeDeg: number, reach: number): number {
        const heading = this.hexapod.rotation.y + (relativeDeg * Math.PI) / 180;
        const dx = -Math.sin(heading);
        const dz = -Math.cos(heading);
        const px = this.hexapod.position.x;
        const pz = this.hexapod.position.z;
        for (let d = PointAndGo.SAMPLE_START; d <= reach; d += PointAndGo.SAMPLE_STEP) {
            if (this.map.isBlocked(px + dx * d, pz + dz * d, PointAndGo.ROBOT_RADIUS)) {
                return d - PointAndGo.SAMPLE_STEP;
            }
        }
        return reach;
    }

    private updateMarker(now: number) {
        if (!this.target) return;
        this.ring.scale.setScalar(1 + 0.12 * Math.sin(now / 160));
        const positions = this.line.geometry.getAttribute('position');
        positions.setXYZ(0, this.hexapod.position.x, PointAndGo.FLOOR_Y, this.hexapod.position.z);
        positions.setXYZ(1, this.target.x, PointAndGo.FLOOR_Y, this.target.z);
        positions.needsUpdate = true;
    }

    private onPointerDown(event: PointerEvent) {
        if (!event.isPrimary || event.button !== 0) return;
        this.downX = event.clientX;
        this.downY = event.clientY;
        this.downAt = performance.now();
    }

    private onPointerUp(event: PointerEvent) {
        if (!event.isPrimary || event.button !== 0 || !this.connected) return;
        const moved = Math.hypot(event.clientX - this.downX, event.clientY - this.downY);
        if (moved > PointAndGo.CLICK_MAX_MOVE_PX || performance.now() - this.downAt > PointAndGo.CLICK_MAX_MS) return;

        const point = this.pickFloor(event);
        if (!point) return;
        if (Math.hypot(point.x - this.hexapod.position.x, point.z - this.hexapod.position.z) > PointAndGo.MAX_DISTANCE) return;
        this.go(point);
    }

    private pickFloor(event: PointerEvent): Vector3 | null {
        const rect = this.canvas.getBoundingClientRect();
        const ndc = new Vector2(
            ((event.clientX - rect.left) / rect.width) * 2 - 1,
            -((event.clientY - rect.top) / rect.height) * 2 + 1,
        );
        this.raycaster.setFromCamera(ndc, this.camera);
        return this.raycaster.ray.intersectPlane(this.floor, new Vector3());
    }

    private go(point: Vector3) {
        this.target = point;
        this.turning = false;
        this.detourDeg = 0;
        this.detourSide = 0;
        this.lastRelaxAt = 0;
        this.lastPlanAt = 0;
        this.startedAt = performance.now();
        this.ring.position.set(point.x, PointAndGo.FLOOR_Y, point.z);
        this.ring.visible = true;
        this.line.visible = true;
        this.updateMarker(this.startedAt);
    }
}

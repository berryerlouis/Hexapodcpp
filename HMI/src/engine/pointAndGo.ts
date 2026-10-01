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

/**
 * Click the floor and the hexapod turns towards the spot, then walks to it.
 * Position comes from the HMI's own foot odometry, so the arrival is approximate.
 * Any manual input, stop or disconnection cancels the trip.
 */
export default class PointAndGo extends Object3D {
    private static readonly ARRIVE_DISTANCE = 0.35;
    private static readonly MAX_DISTANCE = 40;
    private static readonly TURN_ENTER_DEG = 20;
    private static readonly TURN_EXIT_DEG = 8;
    private static readonly NEAR_DISTANCE = 0.8;
    private static readonly SLOW_DISTANCE = 1.5;
    private static readonly TIMEOUT_MS = 120_000;
    private static readonly CLICK_MAX_MOVE_PX = 6;
    private static readonly CLICK_MAX_MS = 350;
    private static readonly FLOOR_Y = 0.03;

    private readonly hexapod: Hexapod;
    private readonly drive: DriveController;
    private readonly camera: PerspectiveCamera;
    private readonly canvas: HTMLCanvasElement;
    private readonly raycaster = new Raycaster();
    private readonly floor = new Plane(new Vector3(0, 1, 0), 0);
    private readonly local = new Vector3();

    private readonly ring: Mesh;
    private readonly line: Line;

    private target: Vector3 | null = null;
    private turning = false;
    private startedAt = 0;
    private connected = false;
    private downX = 0;
    private downY = 0;
    private downAt = 0;

    constructor(hexapod: Hexapod, drive: DriveController, camera: PerspectiveCamera, canvas: HTMLCanvasElement, socket: Socket) {
        super();
        this.hexapod = hexapod;
        this.drive = drive;
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
        if (distance < PointAndGo.ARRIVE_DISTANCE) {
            this.cancel(true);
            return;
        }

        // Positive = target on the left (counter-clockwise turn needed).
        const errorDeg = (Math.atan2(-this.local.x, -this.local.z) * 180) / Math.PI;
        const absError = Math.abs(errorDeg);
        if (!this.turning && absError > PointAndGo.TURN_ENTER_DEG && distance > PointAndGo.NEAR_DISTANCE) {
            this.turning = true;
        } else if (this.turning && absError < PointAndGo.TURN_EXIT_DEG) {
            this.turning = false;
        }

        // The gait cannot walk and turn at once, so the trip alternates between the two.
        if (this.turning) {
            this.drive.setMove(DriveController.AUTO, 0, 0);
            this.drive.setTurn(DriveController.AUTO, -Math.sign(errorDeg) * Math.min(1, Math.max(0.35, absError / 90)));
        } else {
            const speed = Math.min(1, Math.max(0.4, distance / PointAndGo.SLOW_DISTANCE));
            this.drive.setMove(DriveController.AUTO, 0, speed);
            this.drive.setTurn(DriveController.AUTO, 0);
        }

        this.updateMarker(now);
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
        this.startedAt = performance.now();
        this.ring.position.set(point.x, PointAndGo.FLOOR_Y, point.z);
        this.ring.visible = true;
        this.line.visible = true;
        this.updateMarker(this.startedAt);
    }
}

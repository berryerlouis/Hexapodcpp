import {
    BufferAttribute,
    BufferGeometry,
    Camera,
    CanvasTexture,
    DoubleSide,
    Line,
    LineBasicMaterial,
    Mesh,
    MeshBasicMaterial,
    Object3D,
    Raycaster,
    RingGeometry,
    Sprite,
    SpriteMaterial,
    Vector2,
    Vector3,
} from 'three';
import { ClusterName } from "../../communication/clusters/clusterType.ts";
import { ClusterBodyCommands } from "../../communication/clusters/clusterBody.ts";
import Message from "../../communication/message.ts";
import Socket from "../../communication/socket.ts";

/**
 * 3D heading/rotation HUD drawn on the ground around the hexapod.
 *
 *  - Outer (green) ring  : walk direction selector. Click = set direction.
 *  - Inner (orange) ring : turning-rate selector. Click = set rotation
 *                          (left half = CCW, right half = CW).
 *  - Cyan cursor         : desired target heading (target-seek mode).
 *  - Blue 'N' compass    : world north (counter-rotates with bot yaw).
 *  - Yellow hover ghost  : live preview of the angle the click would send.
 *
 * Angle convention (matches the rest of the HMI):
 *   0° = forward (-Z), 90° = left (+X), 180° = back (+Z), 270° = right (-X).
 *   Clockwise viewed from above = signed-negative on the rotation ring.
 */
export default class RotationCircle extends Object3D {
    private static readonly OUTER_RADIUS = 6;
    private static readonly INNER_RADIUS = 3.6;
    private static readonly HIT_HALF_WIDTH_OUTER = 0.9;
    private static readonly HIT_HALF_WIDTH_INNER = 0.8;
    private static readonly CIRCLE_SEGMENTS = 96;

    /** Snap step (degrees) when clicking the rings. */
    private static readonly DIRECTION_SNAP_DEG = 5;
    private static readonly ROTATION_SNAP_DEG = 5;
    /** Dead-zone at the front of the inner ring -> "no rotation". */
    private static readonly ROTATION_DEADZONE_DEG = 4;

    private socket: Socket;
    private camera: Camera | null = null;
    private raycaster: Raycaster = new Raycaster();

    // Outer ring (direction).
    private directionLine: Line;
    private directionHitArea: Mesh;
    private directionTicks: Object3D;
    private directionCursor: Object3D;
    private directionLabels: Object3D;

    // Inner ring (turning rate).
    private rotationLine: Line;
    private rotationHitArea: Mesh;
    private rotationCursor: Object3D;
    private rotationLabels: Object3D;

    // Target-seek + hover preview + compass.
    private targetCursor: Object3D;
    private hoverCursor: Object3D;
    private headingCursor: Object3D;

    private currentDirectionDeg = 0;
    private currentTurningRate = 0;
    private currentClockwise = false;
    private currentHeading = 0;
    private isTargetActive = false;
    private isEnabled = false;

    private readonly clickHandler: (event: MouseEvent) => void;
    private readonly moveHandler: (event: MouseEvent) => void;

    /** Called when the user picks a rotation target on the inner ring. */
    public onRotationTarget: ((offsetDeg: number, clockwise: boolean) => void) | null = null;

    constructor(socket: Socket) {
        super();
        this.socket = socket;

        // Outer ring: direction (green).
        this.directionLine = this.makeCircleLine(RotationCircle.OUTER_RADIUS, 0x39ff7f, 0.75);
        this.directionLine.visible = false;
        this.add(this.directionLine);

        this.directionHitArea = this.makeHitRing(RotationCircle.OUTER_RADIUS, RotationCircle.HIT_HALF_WIDTH_OUTER);
        this.add(this.directionHitArea);

        this.directionTicks = new Object3D();
        this.add(this.directionTicks);
        this.makeTicks(this.directionTicks, RotationCircle.OUTER_RADIUS, 72, 0.55, 0.22);

        this.directionLabels = this.makeAngleLabels(RotationCircle.OUTER_RADIUS + 1.1, '#39ff7f');
        this.add(this.directionLabels);

        this.directionCursor = this.makeCursor(0xff3344, 0.92, RotationCircle.OUTER_RADIUS, 0.55, 0.95);
        this.add(this.directionCursor);

        // Inner ring: turning rate (orange).
        this.rotationLine = this.makeCircleLine(RotationCircle.INNER_RADIUS, 0xffaa33, 0.75);
        this.rotationLine.visible = false;
        this.add(this.rotationLine);

        this.rotationHitArea = this.makeHitRing(RotationCircle.INNER_RADIUS, RotationCircle.HIT_HALF_WIDTH_INNER);
        this.add(this.rotationHitArea);

        this.rotationLabels = this.makeRotationLabels(RotationCircle.INNER_RADIUS - 0.85, '#ffaa33');
        this.add(this.rotationLabels);

        this.rotationCursor = this.makeCursor(0xffaa33, 0.85, RotationCircle.INNER_RADIUS, 0.45, 0.8);
        this.add(this.rotationCursor);

        // Cyan ghost cursor for target-seek mode.
        this.targetCursor = this.makeCursor(0x00ddff, 0.55, RotationCircle.INNER_RADIUS, 0.45, 0.8);
        this.targetCursor.visible = false;
        this.add(this.targetCursor);

        // Yellow hover-preview cursor (sized for outer ring; scaled down when over inner).
        this.hoverCursor = this.makeCursor(0xffe066, 0.65, RotationCircle.OUTER_RADIUS, 0.5, 0.95);
        this.hoverCursor.visible = false;
        this.add(this.hoverCursor);

        // Compass north 'N'.
        this.headingCursor = this.makeNorthCursor();
        this.add(this.headingCursor);

        this.clickHandler = (e) => this.onMouseClick(e);
        this.moveHandler = (e) => this.onMouseMove(e);
    }

    // ── Public API ────────────────────────────────────────────────

    public enable() {
        if (this.isEnabled) return;
        this.isEnabled = true;
        this.setRingsVisible(true);
        window.addEventListener('click', this.clickHandler, false);
        window.addEventListener('mousemove', this.moveHandler, false);
    }

    public disable() {
        if (!this.isEnabled) return;
        this.isEnabled = false;
        this.setRingsVisible(false);
        this.targetCursor.visible = false;
        this.hoverCursor.visible = false;
        window.removeEventListener('click', this.clickHandler, false);
        window.removeEventListener('mousemove', this.moveHandler, false);
    }

    public setCamera(camera: Camera) {
        this.camera = camera;
    }

    /** Update the direction cursor (red arrow, outer ring). */
    public setDirection(directionDeg: number) {
        this.currentDirectionDeg = directionDeg;
        this.directionCursor.rotation.y = Math.PI + this.toRad(this.currentDirectionDeg);
    }

    /** Update the turning-rate cursor (orange arrow, inner ring). */
    public setTurningRate(turningRate: number, clockwise: boolean) {
        this.currentTurningRate = turningRate;
        this.currentClockwise = clockwise;
        this.updateTurningRateCursor();
    }

    /**
     * Show/hide the target-rotation cursor (cyan, inner ring).
     * @param offsetRad Signed offset from forward to target
     *                  (positive = CCW/left, negative = CW/right), or null to hide.
     */
    public setRotationTarget(offsetRad: number | null) {
        if (offsetRad === null) {
            this.isTargetActive = false;
            this.targetCursor.visible = false;
            this.updateTurningRateCursor();
        } else {
            this.isTargetActive = true;
            this.targetCursor.visible = true;
            this.targetCursor.rotation.y = Math.PI + offsetRad;
            this.rotationCursor.rotation.y = Math.PI;
        }
    }

    /** Update the compass-north 'N' indicator. */
    public setHeading(headingDeg: number) {
        const rounded = Math.round(headingDeg);
        if (rounded === this.currentHeading) return;
        const delta = rounded - this.currentHeading;
        this.currentHeading = rounded;
        this.headingCursor.rotation.y = -this.toRad(this.currentHeading);

        // Keep the walk direction fixed in world space as the bot yaws.
        if (this.currentDirectionDeg !== 0 || delta !== 0) {
            const newDir = ((this.currentDirectionDeg - delta) % 360 + 360) % 360;
            this.sendDirection(newDir);
        }
    }

    public dispose() {
        this.disable();
        this.directionLine.geometry.dispose();
        (this.directionLine.material as LineBasicMaterial).dispose();
        this.rotationLine.geometry.dispose();
        (this.rotationLine.material as LineBasicMaterial).dispose();
        this.directionHitArea.geometry.dispose();
        (this.directionHitArea.material as MeshBasicMaterial).dispose();
        this.rotationHitArea.geometry.dispose();
        (this.rotationHitArea.material as MeshBasicMaterial).dispose();
    }

    // ── Geometry builders ─────────────────────────────────────────

    private makeCircleLine(radius: number, color: number, opacity: number): Line {
        const points: Vector3[] = [];
        for (let i = 0; i <= RotationCircle.CIRCLE_SEGMENTS; i++) {
            const a = (i / RotationCircle.CIRCLE_SEGMENTS) * Math.PI * 2;
            points.push(new Vector3(Math.cos(a) * radius, 0, Math.sin(a) * radius));
        }
        const geom = new BufferGeometry().setFromPoints(points);
        return new Line(geom, new LineBasicMaterial({ color, transparent: true, opacity }));
    }

    /** Invisible flat ring used as a reliable click/hover target. */
    private makeHitRing(radius: number, halfWidth: number): Mesh {
        const geom = new RingGeometry(radius - halfWidth, radius + halfWidth, RotationCircle.CIRCLE_SEGMENTS);
        geom.rotateX(-Math.PI / 2);
        const mat = new MeshBasicMaterial({ transparent: true, opacity: 0, side: DoubleSide });
        const mesh = new Mesh(geom, mat);
        mesh.visible = false;
        return mesh;
    }

    private makeTicks(group: Object3D, radius: number, count: number, majorLen: number, minorLen: number) {
        for (let i = 0; i < count; i++) {
            const a = (i / count) * Math.PI * 2;
            const isMajor = i % 6 === 0; // every 30° if count = 72.
            const len = isMajor ? majorLen : minorLen;
            const geom = new BufferGeometry().setFromPoints([
                new Vector3(Math.cos(a) * radius, 0, Math.sin(a) * radius),
                new Vector3(Math.cos(a) * (radius + len), 0, Math.sin(a) * (radius + len)),
            ]);
            group.add(new Line(geom, new LineBasicMaterial({
                color: isMajor ? 0xffffff : 0x9aaab8,
                transparent: true,
                opacity: isMajor ? 0.95 : 0.5,
            })));
        }
    }

    private makeCursor(color: number, opacity: number, radius: number, halfWidth: number, length: number): Object3D {
        const group = new Object3D();
        const vertices = new Float32Array([
            0, 0, radius + length,
            -halfWidth, 0, radius,
             halfWidth, 0, radius,
        ]);
        const indices = new Uint32Array([0, 1, 2, 0, 2, 1]);
        const geom = new BufferGeometry();
        geom.setAttribute('position', new BufferAttribute(vertices, 3));
        geom.setIndex(new BufferAttribute(indices, 1));
        geom.computeVertexNormals();
        const mat = new MeshBasicMaterial({ color, transparent: true, opacity, side: DoubleSide, depthTest: false });
        const mesh = new Mesh(geom, mat);
        mesh.renderOrder = 10;
        group.add(mesh);
        return group;
    }

    /** Numeric labels every 30° around the outer (direction) ring. */
    private makeAngleLabels(radius: number, color: string): Object3D {
        const group = new Object3D();
        for (let deg = 0; deg < 360; deg += 30) {
            group.add(this.makeAngleLabel(`${deg}`, color, 0.6, deg, radius));
        }
        return group;
    }

    /** CW/CCW rotation labels around the inner (turning-rate) ring. */
    private makeRotationLabels(radius: number, color: string): Object3D {
        const group = new Object3D();
        const stops: Array<{ deg: number; text: string }> = [
            { deg: 0,   text: '0°'    },
            { deg: 45,  text: '↺45'  },
            { deg: 90,  text: '↺90'  },
            { deg: 135, text: '↺135' },
            { deg: 180, text: '180°' },
            { deg: 225, text: '↻135' },
            { deg: 270, text: '↻90'  },
            { deg: 315, text: '↻45'  },
        ];
        for (const { deg, text } of stops) {
            group.add(this.makeAngleLabel(text, color, 0.55, deg, radius));
        }
        return group;
    }

    /**
     * Place a text sprite at the angle-deg point on a ring of given radius.
     * Uses the same forward/CCW convention as `setDirection`: the cursor visible
     * at directionDeg is the rotation `Math.PI + toRad(deg)` of a vertex at +Z,
     * which lands at (-sin·r, 0, -cos·r).
     */
    private makeAngleLabel(text: string, color: string, scale: number, deg: number, radius: number): Sprite {
        const sprite = this.makeTextSprite(text, color, scale);
        const rad = this.toRad(deg);
        sprite.position.set(-Math.sin(rad) * radius, 0.15, -Math.cos(rad) * radius);
        return sprite;
    }

    private makeTextSprite(text: string, color: string, scale: number): Sprite {
        const canvas = document.createElement('canvas');
        canvas.width = 128;
        canvas.height = 64;
        const ctx = canvas.getContext('2d')!;
        ctx.fillStyle = color;
        ctx.font = 'bold 44px "Rajdhani", Arial, sans-serif';
        ctx.textAlign = 'center';
        ctx.textBaseline = 'middle';
        ctx.shadowColor = 'rgba(0,0,0,0.85)';
        ctx.shadowBlur = 6;
        ctx.fillText(text, 64, 32);
        const tex = new CanvasTexture(canvas);
        const sprite = new Sprite(new SpriteMaterial({ map: tex, transparent: true, depthTest: false }));
        sprite.scale.set(scale * 2, scale, 1);
        sprite.renderOrder = 11;
        return sprite;
    }

    private makeNorthCursor(): Object3D {
        const group = new Object3D();
        const sprite = this.makeTextSprite('N', '#7ab8ff', 0.9);
        sprite.position.set(0, 0.15, -(RotationCircle.OUTER_RADIUS + 1.9));
        group.add(sprite);
        return group;
    }

    private setRingsVisible(visible: boolean) {
        this.directionLine.visible = visible;
        this.directionHitArea.visible = visible;
        this.directionTicks.visible = visible;
        this.directionCursor.visible = visible;
        this.directionLabels.visible = visible;
        this.rotationLine.visible = visible;
        this.rotationHitArea.visible = visible;
        this.rotationCursor.visible = visible;
        this.rotationLabels.visible = visible;
        this.headingCursor.visible = visible;
    }

    // ── Cursor updates ────────────────────────────────────────────

    private updateTurningRateCursor() {
        if (this.isTargetActive) {
            this.rotationCursor.rotation.y = Math.PI;
            return;
        }
        const rad = this.toRad(this.currentTurningRate);
        const sign = this.currentClockwise ? -1 : 1;
        this.rotationCursor.rotation.y = Math.PI + sign * rad;
    }

    // ── Mouse handling ────────────────────────────────────────────

    private getRayHit(event: MouseEvent, target: Mesh): Vector3 | null {
        if (!this.camera) return null;
        const canvas = document.querySelector('canvas');
        if (!canvas) return null;
        const rect = canvas.getBoundingClientRect();
        const mouse = new Vector2(
            ((event.clientX - rect.left) / rect.width) * 2 - 1,
            -((event.clientY - rect.top) / rect.height) * 2 + 1,
        );
        this.raycaster.setFromCamera(mouse, this.camera);
        const hits = this.raycaster.intersectObject(target);
        return hits.length > 0 ? hits[0].point : null;
    }

    private onMouseMove(event: MouseEvent) {
        if (!this.isEnabled) return;
        const innerPoint = this.getRayHit(event, this.rotationHitArea);
        if (innerPoint) {
            const local = this.worldToLocal(innerPoint.clone());
            const { directionDeg } = this.localPointToAngle(local);
            this.showHover(directionDeg, true);
            return;
        }
        const outerPoint = this.getRayHit(event, this.directionHitArea);
        if (outerPoint) {
            const local = this.worldToLocal(outerPoint.clone());
            const { directionDeg } = this.localPointToAngle(local);
            const snapped = this.snap(directionDeg, RotationCircle.DIRECTION_SNAP_DEG);
            this.showHover(snapped, false);
            return;
        }
        this.hoverCursor.visible = false;
    }

    private onMouseClick(event: MouseEvent) {
        if (!this.isEnabled) return;
        const rotPoint = this.getRayHit(event, this.rotationHitArea);
        if (rotPoint) {
            this.handleRotationClick(rotPoint);
            return;
        }
        const dirPoint = this.getRayHit(event, this.directionHitArea);
        if (dirPoint) {
            this.handleDirectionClick(dirPoint);
        }
    }

    private showHover(displayDeg: number, isInner: boolean) {
        this.hoverCursor.visible = true;
        const scale = isInner ? RotationCircle.INNER_RADIUS / RotationCircle.OUTER_RADIUS : 1;
        this.hoverCursor.scale.set(scale, 1, scale);
        this.hoverCursor.rotation.y = Math.PI + this.toRad(displayDeg);
    }

    private handleDirectionClick(worldPoint: Vector3) {
        const local = this.worldToLocal(worldPoint.clone());
        const { directionDeg } = this.localPointToAngle(local);
        const snapped = this.snap(directionDeg, RotationCircle.DIRECTION_SNAP_DEG);
        this.sendDirection(((snapped % 360) + 360) % 360);
    }

    private handleRotationClick(worldPoint: Vector3) {
        const local = this.worldToLocal(worldPoint.clone());
        const { signedDeg } = this.localPointToAngle(local);
        const snapped = this.snap(signedDeg, RotationCircle.ROTATION_SNAP_DEG);
        const absDeg = Math.abs(snapped);

        if (absDeg < RotationCircle.ROTATION_DEADZONE_DEG) {
            this.sendTurningRate(0, false);
            this.onRotationTarget?.(0, false);
        } else {
            // signedDeg > 0 means click was on the right half = clockwise.
            const clockwise = snapped > 0;
            this.sendTurningRate(absDeg, clockwise);
            this.onRotationTarget?.(absDeg, clockwise);
        }
    }

    /**
     * Convert a local point on the ring's XZ plane into:
     *   - directionDeg in [0, 360) with 0 = forward (-Z), increasing CCW
     *   - signedDeg    in (-180, 180] with 0 = forward, + = right (CW)
     */
    private localPointToAngle(local: Vector3): { directionDeg: number; signedDeg: number } {
        const signedRad = Math.atan2(local.x, -local.z); // 0 = forward, +π/2 = right
        const signedDeg = Math.round((signedRad * 180) / Math.PI);
        let directionDeg = -signedDeg;
        if (directionDeg < 0) directionDeg += 360;
        return { directionDeg, signedDeg };
    }

    private snap(deg: number, step: number): number {
        if (step <= 0) return Math.round(deg);
        return Math.round(deg / step) * step;
    }

    private toRad(deg: number): number {
        return (deg * Math.PI) / 180;
    }

    // ── Protocol senders ──────────────────────────────────────────

    private sendDirection(angleDeg: number) {
        this.setDirection(angleDeg);
        const v = Math.round(angleDeg) & 0xFFFF;
        this.socket.write(new Message(
            ClusterName.BODY,
            ClusterBodyCommands.SET_DIRECTION,
            [v & 0xFF, (v >> 8) & 0xFF],
            [0xFF, 0xFF],
        ));
    }

    private sendTurningRate(turningRate: number, clockwise: boolean) {
        this.setTurningRate(turningRate, clockwise);
        this.socket.write(new Message(
            ClusterName.BODY,
            ClusterBodyCommands.SET_ROTATION,
            [turningRate, clockwise ? 1 : 0],
            [0xFFFF, 0xFF],
        ));
    }
}

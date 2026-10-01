import {
    BufferAttribute,
    BufferGeometry,
    Color,
    ConeGeometry,
    DoubleSide,
    DynamicDrawUsage,
    Group,
    LineBasicMaterial,
    LineSegments,
    Mesh,
    MeshBasicMaterial,
    Object3D,
    Points,
    PointsMaterial,
    RingGeometry,
    Vector3,
} from "three";
import Socket from "../../communication/socket";
import RangeMap from "./rangeMap";
import { CM_PER_UNIT, TOF, US, rangeColor, tofRayDirections } from "./rangeSensors";

export enum ProximitySide {
    left,
    right,
    center
}

// Real sensors come from the socket; the simulator feeds the same pipeline from a virtual room.
export type SensorSource = 'live' | 'sim';

// Samples closer than this are the robot's own legs or the head, not the environment.
const MIN_MAP_CM = 5;
const STALE_MS = 1500;
const ZONES = TOF.size * TOF.size;
// The ultrasound modules sit on the 60 deg faces of the head (sensor frame, forward is -Z).
const US_OFFSET_X = 0.345;
const US_OFFSET_Y = 0.01;
const US_OFFSET_Z = 0.243;

interface UltrasoundBeam {
    group: Group;
    cone: Mesh;
    ring: Mesh;
    coneMaterial: MeshBasicMaterial;
    ringMaterial: MeshBasicMaterial;
}

export interface WorldRay {
    origin: Vector3;
    direction: Vector3;
}

export default class ProximityObject extends Object3D {
    socket: Socket;
    map: RangeMap | null = null;
    source: SensorSource = 'live';

    // Last measurements in cm, 0 meaning no return. Row-major ToF zones as sent by the firmware.
    readonly tofCm = new Float32Array(ZONES);
    readonly ultrasoundCm = [0, 0];
    tofTime = 0;
    readonly ultrasoundTime = [0, 0];

    // World-space copy of the last samples, used by the HUD.
    readonly tofOrigin = new Vector3();
    readonly tofHits = new Float32Array(ZONES * 3);
    readonly ultrasoundOrigin = [new Vector3(), new Vector3()];
    readonly ultrasoundHit = [new Vector3(), new Vector3()];

    private readonly view = new Group();
    private readonly tofGroup = new Group();
    private readonly beams: UltrasoundBeam[];
    private readonly tofDirections: Vector3[];
    private readonly tofPositions = new Float32Array(ZONES * 3);
    private readonly tofColors = new Float32Array(ZONES * 3);
    private readonly rayPositions = new Float32Array(ZONES * 6);
    private readonly rayColors = new Float32Array(ZONES * 6);
    private readonly surfaceIndex = new Uint16Array((TOF.size - 1) * (TOF.size - 1) * 6);
    private readonly surface = new BufferGeometry();
    private readonly rays = new BufferGeometry();
    private readonly color = new Color();
    private readonly point = new Vector3();

    constructor(socket: Socket) {
        super();
        this.socket = socket;
        this.tofDirections = tofRayDirections().flat();

        this.add(this.view);
        this.view.add(this.createFrustum());

        this.surface.setAttribute('position', new BufferAttribute(this.tofPositions, 3).setUsage(DynamicDrawUsage));
        this.surface.setAttribute('color', new BufferAttribute(this.tofColors, 3).setUsage(DynamicDrawUsage));
        this.surface.setIndex(new BufferAttribute(this.surfaceIndex, 1).setUsage(DynamicDrawUsage));
        this.surface.setDrawRange(0, 0);
        const surfaceMesh = new Mesh(this.surface, new MeshBasicMaterial({
            vertexColors: true,
            transparent: true,
            opacity: 0.32,
            side: DoubleSide,
            depthWrite: false,
            toneMapped: false,
        }));
        surfaceMesh.frustumCulled = false;
        this.tofGroup.add(surfaceMesh);

        this.rays.setAttribute('position', new BufferAttribute(this.rayPositions, 3).setUsage(DynamicDrawUsage));
        this.rays.setAttribute('color', new BufferAttribute(this.rayColors, 3).setUsage(DynamicDrawUsage));
        const rayLines = new LineSegments(this.rays, new LineBasicMaterial({
            vertexColors: true,
            transparent: true,
            opacity: 0.35,
            depthWrite: false,
            toneMapped: false,
        }));
        rayLines.frustumCulled = false;
        this.tofGroup.add(rayLines);

        const hits = new BufferGeometry();
        hits.setAttribute('position', this.surface.getAttribute('position'));
        hits.setAttribute('color', this.surface.getAttribute('color'));
        const hitPoints = new Points(hits, new PointsMaterial({
            vertexColors: true,
            size: 0.1,
            sizeAttenuation: true,
            toneMapped: false,
        }));
        hitPoints.frustumCulled = false;
        this.tofGroup.add(hitPoints);
        this.tofGroup.visible = false;
        this.view.add(this.tofGroup);

        this.beams = [this.createBeam(1), this.createBeam(-1)];
        for (const beam of this.beams) {
            this.view.add(beam.group);
        }
    }

    setMap(map: RangeMap) {
        this.map = map;
    }

    set liveBeams(visible: boolean) {
        this.view.visible = visible;
    }

    get liveBeams(): boolean {
        return this.view.visible;
    }

    isTofFresh(now: number = performance.now()): boolean {
        return this.tofTime > 0 && now - this.tofTime < STALE_MS;
    }

    isUltrasoundFresh(index: number, now: number = performance.now()): boolean {
        return this.ultrasoundTime[index] > 0 && now - this.ultrasoundTime[index] < STALE_MS;
    }

    // World-space rays of the 64 ToF zones, row-major.
    getTofWorldRays(origin: Vector3, directions: Vector3[]) {
        this.updateWorldMatrix(true, false);
        origin.setFromMatrixPosition(this.matrixWorld);
        for (let i = 0; i < ZONES; i++) {
            directions[i] = (directions[i] ?? new Vector3())
                .copy(this.tofDirections[i])
                .transformDirection(this.matrixWorld);
        }
    }

    // index 0 is the left ultrasound sensor, 1 the right one.
    getUltrasoundWorldRay(index: number): WorldRay {
        this.updateWorldMatrix(true, false);
        const beam = this.beams[index];
        beam.group.updateWorldMatrix(true, false);
        return {
            origin: new Vector3().setFromMatrixPosition(beam.group.matrixWorld),
            direction: new Vector3(0, 0, -1).transformDirection(beam.group.matrixWorld),
        };
    }

    show(distance: number | number[][], side: ProximitySide, source: SensorSource = 'live') {
        if (source !== this.source) {
            return;
        }
        if (side === ProximitySide.center && Array.isArray(distance)) {
            this.showTof(distance);
        } else if (typeof distance === 'number' && side !== ProximitySide.center) {
            this.showUltrasound(side === ProximitySide.left ? 0 : 1, distance);
        }
    }

    update() {
        const now = performance.now();
        this.tofGroup.visible = this.isTofFresh(now);
        for (let i = 0; i < this.beams.length; i++) {
            this.beams[i].group.visible = this.isUltrasoundFresh(i, now) && this.ultrasoundCm[i] > 0;
        }
    }

    // Drops the live measurements and the map.
    resetTraces() {
        this.tofCm.fill(0);
        this.ultrasoundCm[0] = this.ultrasoundCm[1] = 0;
        this.tofTime = this.ultrasoundTime[0] = this.ultrasoundTime[1] = 0;
        this.map?.reset();
    }

    private showTof(matrixCm: number[][]) {
        this.updateWorldMatrix(true, false);
        const world = this.matrixWorld;
        this.tofOrigin.setFromMatrixPosition(world);
        this.tofTime = performance.now();

        for (let row = 0; row < TOF.size; row++) {
            for (let col = 0; col < TOF.size; col++) {
                const i = row * TOF.size + col;
                const cm = matrixCm[row][col];
                this.tofCm[i] = cm >= TOF.minCm && cm <= TOF.maxCm ? cm : 0;
                this.writeZone(i, this.tofCm[i]);

                if (this.tofCm[i] === 0) {
                    continue;
                }
                this.point.copy(this.tofDirections[i]).multiplyScalar(this.tofCm[i] / CM_PER_UNIT).applyMatrix4(world);
                this.tofHits[i * 3] = this.point.x;
                this.tofHits[i * 3 + 1] = this.point.y;
                this.tofHits[i * 3 + 2] = this.point.z;
                this.integrate(this.tofOrigin, this.point, this.tofCm[i]);
            }
        }
        this.buildSurface();
        this.surface.getAttribute('position').needsUpdate = true;
        this.surface.getAttribute('color').needsUpdate = true;
        this.rays.getAttribute('position').needsUpdate = true;
        this.rays.getAttribute('color').needsUpdate = true;
    }

    private showUltrasound(index: number, cm: number) {
        const valid = cm >= US.minCm && cm <= US.maxCm;
        this.ultrasoundCm[index] = valid ? cm : 0;
        this.ultrasoundTime[index] = performance.now();
        if (!valid) {
            return;
        }

        const beam = this.beams[index];
        const length = cm / CM_PER_UNIT;
        const radius = length * Math.tan((US.halfAngleDeg * Math.PI) / 180);
        rangeColor(cm, this.color);
        beam.cone.scale.set(radius, radius, length);
        beam.ring.position.z = -length;
        beam.ring.scale.set(radius, radius, 1);
        beam.coneMaterial.color.copy(this.color);
        beam.ringMaterial.color.copy(this.color);

        const ray = this.getUltrasoundWorldRay(index);
        this.ultrasoundOrigin[index].copy(ray.origin);
        this.ultrasoundHit[index].copy(ray.origin).addScaledVector(ray.direction, length);
        this.integrate(ray.origin, this.ultrasoundHit[index], cm);
    }

    // Feeds the map with a world-space hit.
    private integrate(origin: Vector3, hit: Vector3, cm: number) {
        if (!this.map || cm < MIN_MAP_CM || hit.y < -0.6 || hit.y > RangeMap.MAX_HEIGHT) {
            return;
        }
        this.map.addRay(origin, hit, hit.y > RangeMap.FLOOR_TOLERANCE ? 'obstacle' : 'floor');
    }

    // Sensor-frame geometry of one zone; no return collapses it onto the sensor.
    private writeZone(i: number, cm: number) {
        const d = this.tofDirections[i];
        const length = cm > 0 ? cm / CM_PER_UNIT : 0;
        rangeColor(cm, this.color);
        const { r, g, b } = this.color;

        this.tofPositions[i * 3] = this.rayPositions[i * 6 + 3] = d.x * length;
        this.tofPositions[i * 3 + 1] = this.rayPositions[i * 6 + 4] = d.y * length;
        this.tofPositions[i * 3 + 2] = this.rayPositions[i * 6 + 5] = d.z * length;
        this.rayPositions[i * 6] = this.rayPositions[i * 6 + 1] = this.rayPositions[i * 6 + 2] = 0;

        this.tofColors[i * 3] = this.rayColors[i * 6 + 3] = r;
        this.tofColors[i * 3 + 1] = this.rayColors[i * 6 + 4] = g;
        this.tofColors[i * 3 + 2] = this.rayColors[i * 6 + 5] = b;
        this.rayColors[i * 6] = r * 0.4;
        this.rayColors[i * 6 + 1] = g * 0.4;
        this.rayColors[i * 6 + 2] = b * 0.4;
    }

    // Joins neighbouring zones of similar depth into a translucent depth surface.
    private buildSurface() {
        const n = TOF.size;
        let count = 0;
        for (let r = 0; r < n - 1; r++) {
            for (let c = 0; c < n - 1; c++) {
                const a = r * n + c;
                const b = a + 1;
                const d = a + n;
                const e = d + 1;
                const da = this.tofCm[a], db = this.tofCm[b], dd = this.tofCm[d], de = this.tofCm[e];
                if (da === 0 || db === 0 || dd === 0 || de === 0) {
                    continue;
                }
                const min = Math.min(da, db, dd, de);
                if (Math.max(da, db, dd, de) - min > 6 + 0.2 * min) {
                    continue;
                }
                this.surfaceIndex.set([a, d, b, b, d, e], count);
                count += 6;
            }
        }
        this.surface.index!.needsUpdate = true;
        this.surface.setDrawRange(0, count);
    }

    // Wireframe pyramid showing where the ToF is looking even when nothing is detected.
    private createFrustum(): LineSegments {
        const half = Math.tan((TOF.fovDeg * Math.PI) / 360) * 3;
        const corners = [
            new Vector3(-half, half, -3),
            new Vector3(half, half, -3),
            new Vector3(half, -half, -3),
            new Vector3(-half, -half, -3),
        ];
        const points: Vector3[] = [];
        for (let i = 0; i < 4; i++) {
            points.push(new Vector3(), corners[i], corners[i], corners[(i + 1) % 4]);
        }
        return new LineSegments(
            new BufferGeometry().setFromPoints(points),
            new LineBasicMaterial({ color: '#2ee6d6', transparent: true, opacity: 0.22, toneMapped: false }),
        );
    }

    // side 1 is the left module, -1 the right one.
    private createBeam(side: 1 | -1): UltrasoundBeam {
        const group = new Group();
        group.position.set(-side * US_OFFSET_X, US_OFFSET_Y, US_OFFSET_Z);
        group.rotation.y = (side * US.yawDeg * Math.PI) / 180;

        // Apex on the sensor, axis along -Z, unit length.
        const coneGeometry = new ConeGeometry(1, 1, 28, 1, true);
        coneGeometry.translate(0, -0.5, 0);
        coneGeometry.rotateX(Math.PI / 2);
        const coneMaterial = new MeshBasicMaterial({
            color: '#2ee6d6',
            transparent: true,
            opacity: 0.08,
            side: DoubleSide,
            depthWrite: false,
            toneMapped: false,
        });
        const cone = new Mesh(coneGeometry, coneMaterial);
        cone.frustumCulled = false;

        const ringMaterial = new MeshBasicMaterial({
            color: '#2ee6d6',
            transparent: true,
            opacity: 0.6,
            side: DoubleSide,
            depthWrite: false,
            toneMapped: false,
        });
        const ring = new Mesh(new RingGeometry(0.88, 1, 40), ringMaterial);
        ring.frustumCulled = false;

        group.add(cone, ring);
        group.visible = false;
        return { group, cone, ring, coneMaterial, ringMaterial };
    }
}

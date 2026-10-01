import {
    BoxGeometry,
    BufferAttribute,
    BufferGeometry,
    CanvasTexture,
    Color,
    DynamicDrawUsage,
    InstancedMesh,
    Line,
    LineBasicMaterial,
    LinearFilter,
    Matrix4,
    Mesh,
    MeshBasicMaterial,
    MeshStandardMaterial,
    Object3D,
    PlaneGeometry,
    Quaternion,
    SRGBColorSpace,
    Vector3,
} from "three";

export type HitKind = 'floor' | 'obstacle';

// World-fixed occupancy map filled by the range sensors: a floor overlay of explored space,
// obstacle columns (floor up to the highest hit) and the path walked by the hexapod.
export default class RangeMap extends Object3D {
    static readonly VOXEL = 0.5;
    static readonly CELL = 0.25;
    static readonly GRID = 512;
    // Hits within this height of the floor are the floor itself, not an obstacle.
    static readonly FLOOR_TOLERANCE = 0.35;
    static readonly MAX_HEIGHT = 30;

    private static readonly MAX_COLUMNS = 20000;
    private static readonly MIN_HITS = 2;
    private static readonly MAX_TRAIL = 4096;
    private static readonly UPLOAD_INTERVAL_MS = 100;

    private static readonly UNKNOWN = 0;
    private static readonly FREE = 1;
    private static readonly OBSTACLE = 2;

    readonly canvas: HTMLCanvasElement;
    private readonly context: CanvasRenderingContext2D;
    private readonly image: ImageData;
    private readonly state: Uint8Array;
    private readonly texture: CanvasTexture;
    private readonly floor: Mesh;

    private readonly columns: InstancedMesh;
    private readonly columnHits = new Map<number, number>();
    private readonly columnSlot = new Map<number, number>();
    private readonly columnTop = new Float32Array(RangeMap.MAX_COLUMNS);
    private columnCount = 0;
    private readonly columnMatrix = new Matrix4();
    private readonly columnPosition = new Vector3();
    private readonly columnScale = new Vector3(1, 1, 1);
    private readonly columnRotation = new Quaternion();
    private readonly color = new Color();
    private readonly low = new Color('#ffb454');
    private readonly high = new Color('#ff3b5c');

    private readonly trail: Line;
    private readonly trailPoints = new Float32Array(RangeMap.MAX_TRAIL * 3);
    private trailCount = 0;

    private exploredCells = 0;
    private dirty = false;
    private lastUpload = 0;

    constructor() {
        super();
        this.name = 'range-map';

        const grid = RangeMap.GRID;
        this.canvas = document.createElement('canvas');
        this.canvas.width = this.canvas.height = grid;
        this.context = this.canvas.getContext('2d')!;
        this.image = this.context.createImageData(grid, grid);
        this.state = new Uint8Array(grid * grid);

        this.texture = new CanvasTexture(this.canvas);
        this.texture.colorSpace = SRGBColorSpace;
        this.texture.generateMipmaps = false;
        this.texture.minFilter = LinearFilter;
        this.texture.magFilter = LinearFilter;

        this.floor = new Mesh(
            new PlaneGeometry(grid * RangeMap.CELL, grid * RangeMap.CELL),
            new MeshBasicMaterial({ map: this.texture, transparent: true, depthWrite: false, toneMapped: false }),
        );
        this.floor.rotation.x = -Math.PI / 2;
        this.floor.position.y = 0.02;
        this.floor.renderOrder = 1;
        this.add(this.floor);

        const size = RangeMap.VOXEL * 0.92;
        this.columns = new InstancedMesh(
            new BoxGeometry(size, 1, size),
            new MeshStandardMaterial({ roughness: 0.5, metalness: 0.1 }),
            RangeMap.MAX_COLUMNS,
        );
        this.columns.instanceMatrix.setUsage(DynamicDrawUsage);
        this.columns.setColorAt(0, this.color.set('#ffffff'));
        this.columns.count = 0;
        this.columns.frustumCulled = false;
        this.add(this.columns);

        const trailGeometry = new BufferGeometry();
        trailGeometry.setAttribute('position', new BufferAttribute(this.trailPoints, 3).setUsage(DynamicDrawUsage));
        trailGeometry.setDrawRange(0, 0);
        this.trail = new Line(trailGeometry, new LineBasicMaterial({ color: '#2ee6d6', transparent: true, opacity: 0.85, toneMapped: false }));
        this.trail.frustumCulled = false;
        this.trail.position.y = 0.04;
        this.add(this.trail);
    }

    get obstacleBlocks(): number {
        return this.columnCount;
    }

    get exploredAreaM2(): number {
        const cellM = (RangeMap.CELL * 10) / 100;
        return this.exploredCells * cellM * cellM;
    }

    // Records one range sample: free space up to the hit and, for an obstacle, the hit itself.
    addRay(origin: Vector3, hit: Vector3, kind: HitKind) {
        this.markFreeLine(origin.x, origin.z, hit.x, hit.z);
        if (kind === 'obstacle') {
            this.markObstacle(hit.x, hit.z);
            this.addColumn(hit);
        }
    }

    addTrailPoint(x: number, z: number) {
        const p = this.trailPoints;
        const n = this.trailCount;
        if (n > 0) {
            const dx = x - p[(n - 1) * 3];
            const dz = z - p[(n - 1) * 3 + 2];
            if (dx * dx + dz * dz < 0.16) {
                return;
            }
        }
        if (n >= RangeMap.MAX_TRAIL) {
            // Keep the whole path by dropping every other point.
            for (let i = 0; i < n / 2; i++) {
                p.copyWithin(i * 3, i * 6, i * 6 + 3);
            }
            this.trailCount = Math.floor(n / 2);
        }
        const i = this.trailCount++;
        p[i * 3] = x;
        p[i * 3 + 1] = 0;
        p[i * 3 + 2] = z;
        const attribute = this.trail.geometry.getAttribute('position') as BufferAttribute;
        attribute.needsUpdate = true;
        this.trail.geometry.setDrawRange(0, this.trailCount);
    }

    update(now: number) {
        if (this.dirty && now - this.lastUpload >= RangeMap.UPLOAD_INTERVAL_MS) {
            this.context.putImageData(this.image, 0, 0);
            this.texture.needsUpdate = true;
            this.dirty = false;
            this.lastUpload = now;
        }
    }

    reset() {
        this.image.data.fill(0);
        this.state.fill(0);
        this.context.clearRect(0, 0, RangeMap.GRID, RangeMap.GRID);
        this.texture.needsUpdate = true;
        this.exploredCells = 0;
        this.dirty = false;

        this.columnHits.clear();
        this.columnSlot.clear();
        this.columnCount = 0;
        this.columns.count = 0;

        this.trailCount = 0;
        this.trail.geometry.setDrawRange(0, 0);
    }

    // One column per floor cell, standing on the floor up to the highest hit seen there.
    private addColumn(p: Vector3) {
        const v = RangeMap.VOXEL;
        const ix = Math.floor(p.x / v);
        const iz = Math.floor(p.z / v);
        if (Math.abs(ix) > 1000 || Math.abs(iz) > 1000) {
            return;
        }

        const key = (ix + 1024) * 2048 + (iz + 1024);
        let slot = this.columnSlot.get(key);
        if (slot === undefined) {
            const hits = (this.columnHits.get(key) ?? 0) + 1;
            this.columnHits.set(key, hits);
            if (hits < RangeMap.MIN_HITS || this.columnCount >= RangeMap.MAX_COLUMNS) {
                return;
            }
            slot = this.columnCount++;
            this.columnSlot.set(key, slot);
            this.columnTop[slot] = 0;
            this.columns.count = this.columnCount;
        }

        const top = p.y + v / 2;
        if (top <= this.columnTop[slot] + 0.2) {
            return;
        }
        this.columnTop[slot] = top;
        this.columnPosition.set((ix + 0.5) * v, top / 2, (iz + 0.5) * v);
        this.columnScale.set(1, top, 1);
        this.columnMatrix.compose(this.columnPosition, this.columnRotation, this.columnScale);
        this.columns.setMatrixAt(slot, this.columnMatrix);
        this.color.copy(this.low).lerp(this.high, Math.min(1, top / 15));
        this.columns.setColorAt(slot, this.color);
        this.columns.instanceMatrix.needsUpdate = true;
        this.columns.instanceColor!.needsUpdate = true;
    }

    private cellIndex(x: number, z: number): number {
        const grid = RangeMap.GRID;
        const px = Math.floor(x / RangeMap.CELL + grid / 2);
        const py = Math.floor(z / RangeMap.CELL + grid / 2);
        if (px < 0 || py < 0 || px >= grid || py >= grid) {
            return -1;
        }
        return py * grid + px;
    }

    private setCell(index: number, value: number) {
        const current = this.state[index];
        if (current === RangeMap.OBSTACLE || current === value) {
            return;
        }
        if (current === RangeMap.UNKNOWN) {
            this.exploredCells++;
        }
        this.state[index] = value;
        const d = this.image.data;
        const o = index * 4;
        if (value === RangeMap.OBSTACLE) {
            d[o] = 255; d[o + 1] = 93; d[o + 2] = 108; d[o + 3] = 235;
        } else {
            d[o] = 46; d[o + 1] = 230; d[o + 2] = 214; d[o + 3] = 64;
        }
        this.dirty = true;
    }

    private markObstacle(x: number, z: number) {
        const step = RangeMap.CELL;
        for (let dx = -1; dx <= 1; dx++) {
            for (let dz = -1; dz <= 1; dz++) {
                const index = this.cellIndex(x + dx * step, z + dz * step);
                if (index >= 0) {
                    this.setCell(index, RangeMap.OBSTACLE);
                }
            }
        }
    }

    private markFreeLine(x0: number, z0: number, x1: number, z1: number) {
        const cell = RangeMap.CELL;
        const dx = x1 - x0;
        const dz = z1 - z0;
        const steps = Math.ceil(Math.hypot(dx, dz) / cell);
        for (let i = 0; i < steps; i++) {
            const t = i / steps;
            const index = this.cellIndex(x0 + dx * t, z0 + dz * t);
            if (index >= 0) {
                this.setCell(index, RangeMap.FREE);
            }
        }
    }
}

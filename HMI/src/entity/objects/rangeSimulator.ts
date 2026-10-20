import { Box3, Ray, Vector3 } from "three";
import ProximityObject, { ProximitySide } from "./proximity";
import { CM_PER_UNIT, TOF, US } from "./rangeSensors";

const box = (x0: number, y0: number, z0: number, x1: number, y1: number, z1: number) =>
    new Box3(new Vector3(x0, y0, z0), new Vector3(x1, y1, z1));

// Hidden demo room (scene units, 1 = 10 cm) around the spawn point: 6 x 4.4 m with a few
// obstacles. It is never drawn, the sensors have to discover it.
const ROOM = [
    box(-31, 0, -23, 31, 25, -22),
    box(-31, 0, 22, 31, 25, 23),
    box(-31, 0, -23, -30, 25, 23),
    box(30, 0, -23, 31, 25, 23),
    box(8, 0, -8, 16, 7, -2),
    box(-14, 0, 6, -9, 4, 11),
    box(18, 0, 10, 20, 25, 12),
    box(-22, 0, -18, -12, 1.5, -14),
    box(-4, 0, 12, -2, 25, 22),
];

const TOF_PERIOD_MS = 66;
const US_PERIOD_MS = 120;
const US_SPREAD = (12 * Math.PI) / 180;
const UP = new Vector3(0, 1, 0);

// Feeds the proximity pipeline with ranges raycast from the real sensor poses of the model, so the
// map can be tried without hardware.
export default class RangeSimulator {
    enabled = false;

    private readonly proximity: ProximityObject;
    private readonly ray = new Ray();
    private readonly hit = new Vector3();
    private readonly origin = new Vector3();
    private readonly directions: Vector3[] = [];
    private readonly spread = new Vector3();
    private lastTof = 0;
    private lastUltrasound = 0;
    private nextSide = 0;

    constructor(proximity: ProximityObject) {
        this.proximity = proximity;
    }

    update(now: number) {
        if (!this.enabled) {
            return;
        }
        if (now - this.lastTof >= TOF_PERIOD_MS) {
            this.lastTof = now;
            this.sampleTof();
        }
        if (now - this.lastUltrasound >= US_PERIOD_MS) {
            this.lastUltrasound = now;
            this.sampleUltrasound(this.nextSide);
            this.nextSide = 1 - this.nextSide;
        }
    }

    private sampleTof() {
        this.proximity.getTofWorldRays(this.origin, this.directions);
        const matrix: number[][] = [];
        for (let row = 0; row < TOF.size; row++) {
            matrix[row] = [];
            for (let col = 0; col < TOF.size; col++) {
                matrix[row][col] = this.cast(this.origin, this.directions[row * TOF.size + col], TOF.maxCm);
            }
        }
        this.proximity.show(matrix, ProximitySide.center, 'sim');
    }

    private sampleUltrasound(index: number) {
        const { origin, direction } = this.proximity.getUltrasoundWorldRay(index);
        let nearest = 0;
        for (const angle of [0, US_SPREAD, -US_SPREAD]) {
            const cm = this.cast(origin, this.spread.copy(direction).applyAxisAngle(UP, angle), US.maxCm);
            if (cm > 0 && (nearest === 0 || cm < nearest)) {
                nearest = cm;
            }
        }
        this.proximity.show(Math.round(nearest), index === 0 ? ProximitySide.left : ProximitySide.right, 'sim');
    }

    // Range in cm to the first surface, 0 when nothing is within maxCm.
    private cast(origin: Vector3, direction: Vector3, maxCm: number): number {
        let nearest = Infinity;
        if (direction.y < -1e-4 && origin.y > 0) {
            nearest = -origin.y / direction.y;
        }
        this.ray.set(origin, direction);
        for (const obstacle of ROOM) {
            if (this.ray.intersectBox(obstacle, this.hit)) {
                nearest = Math.min(nearest, origin.distanceTo(this.hit));
            }
        }
        const cm = nearest * CM_PER_UNIT * (1 + (Math.random() - 0.5) * 0.01);
        return cm <= maxCm ? cm : 0;
    }
}

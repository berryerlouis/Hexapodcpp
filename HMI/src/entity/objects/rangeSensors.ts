import { Color, Vector3 } from "three";

// Scene unit = 10 cm, so a distance in cm is divided by CM_PER_UNIT.
export const CM_PER_UNIT = 10;

// Sensor frame (ProximityObject): forward is -Z, up is +Y, right is +X.
export const TOF = {
    size: 8,
    fovDeg: 45,
    minCm: 2,
    maxCm: 400,
    // Zone 0 is on the right when looking out of the VL53L5CX; flip these if the picture is mirrored.
    flipColumns: true,
    flipRows: false,
};

export const US = {
    halfAngleDeg: 15,
    minCm: 2,
    maxCm: 400,
    yawDeg: 60,
};

// Firmware defaults, restored when the long range mode is switched off.
export const DEFAULT_THRESHOLD = { usCm: 30, tofMm: 1000 };
export const LONG_RANGE_THRESHOLD = { usCm: US.maxCm, tofMm: TOF.maxCm * 10 };

// Unit ray of every ToF zone, [row][col] as received from the firmware.
export function tofRayDirections(): Vector3[][] {
    const half = Math.tan((TOF.fovDeg * Math.PI) / 360);
    const rays: Vector3[][] = [];
    for (let row = 0; row < TOF.size; row++) {
        rays[row] = [];
        for (let col = 0; col < TOF.size; col++) {
            const c = TOF.flipColumns ? TOF.size - 1 - col : col;
            const r = TOF.flipRows ? TOF.size - 1 - row : row;
            const u = ((c + 0.5) / TOF.size) * 2 - 1;
            const v = 1 - ((r + 0.5) / TOF.size) * 2;
            rays[row][col] = new Vector3(u * half, v * half, -1).normalize();
        }
    }
    return rays;
}

const STOPS: { cm: number; color: Color }[] = [
    { cm: 0, color: new Color('#ff3b5c') },
    { cm: 40, color: new Color('#ff9f43') },
    { cm: 100, color: new Color('#f5d74a') },
    { cm: 200, color: new Color('#3ddc97') },
    { cm: 400, color: new Color('#2ee6d6') },
];

// Near is red, far is cyan.
export function rangeColor(cm: number, out: Color = new Color()): Color {
    if (cm <= STOPS[0].cm) {
        return out.copy(STOPS[0].color);
    }
    for (let i = 1; i < STOPS.length; i++) {
        if (cm <= STOPS[i].cm) {
            const a = STOPS[i - 1];
            const b = STOPS[i];
            return out.copy(a.color).lerp(b.color, (cm - a.cm) / (b.cm - a.cm));
        }
    }
    return out.copy(STOPS[STOPS.length - 1].color);
}

const scratch = new Color();

export function rangeCss(cm: number): string {
    return `#${rangeColor(cm, scratch).getHexString()}`;
}

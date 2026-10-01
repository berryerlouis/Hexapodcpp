import {
    ExtrudeGeometry,
    Object3D,
    Path,
    Shape,
    Vector2,
} from 'three'
import Legs from "./legs.ts";
import Socket from "../../communication/socket.ts";
import { ClusterName } from "../../communication/clusters/clusterType.ts";
import { ClusterServoCommands } from "../../communication/clusters/clusterServo.ts";
import Message from "../../communication/message.ts";
import { LEG, Mat, ModelBuilder } from "./materials.ts";

interface BodyStruct {
    width: number;
    widthMiddle: number;
    height: number;
    thickness: number;
    color: string;
}

function convexHull(points: Vector2[]): Vector2[] {
    const sorted = [...points].sort((a, b) => a.x - b.x || a.y - b.y);
    const cross = (o: Vector2, a: Vector2, b: Vector2) => (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
    const lower: Vector2[] = [];
    for (const p of sorted) {
        while (lower.length >= 2 && cross(lower[lower.length - 2], lower[lower.length - 1], p) <= 0) lower.pop();
        lower.push(p);
    }
    const upper: Vector2[] = [];
    for (let i = sorted.length - 1; i >= 0; i--) {
        const p = sorted[i];
        while (upper.length >= 2 && cross(upper[upper.length - 2], upper[upper.length - 1], p) <= 0) upper.pop();
        upper.push(p);
    }
    lower.pop();
    upper.pop();
    return lower.concat(upper);
}

// Convex outline made of one circle of the given radius around each point.
function roundedHull(points: [number, number][], radius: number, samples = 20): Vector2[] {
    const cloud: Vector2[] = [];
    for (const [px, py] of points) {
        for (let i = 0; i < samples; i++) {
            const t = (Math.PI * 2 * i) / samples;
            cloud.push(new Vector2(px + radius * Math.cos(t), py + radius * Math.sin(t)));
        }
    }
    return convexHull(cloud);
}

function roundedRect(cx: number, cy: number, w: number, h: number, r: number): Path {
    const x = cx - w / 2;
    const y = cy - h / 2;
    const p = new Path();
    p.moveTo(x + r, y);
    p.lineTo(x + w - r, y);
    p.quadraticCurveTo(x + w, y, x + w, y + r);
    p.lineTo(x + w, y + h - r);
    p.quadraticCurveTo(x + w, y + h, x + w - r, y + h);
    p.lineTo(x + r, y + h);
    p.quadraticCurveTo(x, y + h, x, y + h - r);
    p.lineTo(x, y + r);
    p.quadraticCurveTo(x, y, x + r, y);
    return p;
}

function roundHole(cx: number, cy: number, r: number): Path {
    const p = new Path();
    p.absarc(cx, cy, r, 0, Math.PI * 2, true);
    return p;
}

export default class Body extends Object3D {

    interval: number | null = null;
    params: BodyStruct = {
        width: 0.8,
        widthMiddle: 1.3,
        height: 1.5,
        thickness: 0.2,
        color: '#515151'
    }
    x: number = 0;
    y: number = 0;
    z: number = 0;
    enable: HTMLElement;
    socket: Socket;
    members: Legs;
    intervalEnable: number = 0;
    intervalDisable: number = 0;
    intervalTime: number = 0;

    constructor(x: number, y: number, z: number, socket: Socket, intervalCommand: number) {
        super();
        this.intervalEnable = intervalCommand;
        this.intervalDisable = 1000;
        this.intervalTime = this.intervalEnable;
        this.x = x;
        this.y = y;
        this.z = z;
        this.socket = socket;

        this.members = new Legs(this.x, this.y, this.z, this.params.width, this.params.widthMiddle, this.params.height, this.socket);
        this.add(this.members);

        this.drawBody();

        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_ALL, (message: Message) => {
            if (message.params && message.params.length === 18) {
                for (let i = 0; i < 6; i++) {
                    for (let j = 0; j < 3; j++) {
                        this.members.legs.leg[i].legData.servos[j].angle = message.getValueUint8(i * 3 + j);
                    }
                }
            }
        });

        this.enable = document.getElementById('enable')!;
        this.enable.addEventListener('click', () => {
            if (this.enable.classList.contains('select')) {
                this.intervalTime = this.intervalEnable;
                if (this.interval !== null) {
                    clearInterval(this.interval);
                }
                this.interval = setInterval(() => {
                    this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_ALL));
                }, this.intervalEnable);
            } else {
                this.intervalTime = this.intervalDisable;
                if (this.interval !== null) {
                    clearInterval(this.interval);
                }
                this.interval = setInterval(() => {
                    this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_ALL));
                }, this.intervalDisable);
            }
        });

        this.socket.addCallbackStarted(() => {
            // if (this.interval !== null) {
            //     clearInterval(this.interval);
            // }
            // this.interval = setInterval(() => {
            //     this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_ALL));
            // }, this.intervalTime);
        });

        this.socket.addCallbackStopped(() => {
            if (this.interval !== null) {
                clearInterval(this.interval);
                this.interval = null;
            }
        });
    }

    // Capers II frame: two CNC aluminium plates held 50 mm apart by six brass standoffs. The coxa
    // servo horns bolt to the top plate and the bearings ride in the bottom plate. The electronics
    // hang under the top plate and the battery is strapped under the bottom plate.
    drawBody(): void {
        const y0 = this.y;
        const h = this.params.height;
        const w = this.params.width;
        const wm = this.params.widthMiddle;
        const T = LEG.plateThickness;
        const topY = y0 - LEG.topPlateZ;
        const bottomY = y0 - LEG.bottomPlateZ;

        const mounts: [number, number][] = [
            [ h / 2, -w / 2 ],
            [ h / 2,  w / 2 ],
            [ 0,      wm / 2 ],
            [-h / 2,  w / 2 ],
            [-h / 2, -w / 2 ],
            [ 0,     -wm / 2 ],
        ];
        const outline = roundedHull(mounts, 0.19);

        // Shape y maps to world -z; every hole below is symmetric in z.
        const makePlate = (holes: Path[]): ExtrudeGeometry => {
            const shape = new Shape(outline.map((p) => new Vector2(p.x, -p.y)));
            shape.holes.push(...holes);
            const geometry = new ExtrudeGeometry(shape, {
                depth: T - 0.008,
                bevelEnabled: true,
                bevelThickness: 0.004,
                bevelSize: 0.004,
                bevelSegments: 1,
                curveSegments: 8,
            });
            geometry.rotateX(-Math.PI / 2);
            geometry.translate(0, -(T - 0.008) / 2, 0);
            return geometry;
        };

        const b = new ModelBuilder();
        b.add(makePlate([roundedRect(0, 0, 0.58, 0.24, 0.05), roundHole(-0.72, 0, 0.04)]), Mat.alu, 0, topY, 0);
        b.add(makePlate([roundedRect(-0.25, 0, 0.05, 0.3, 0.02), roundedRect(0.25, 0, 0.05, 0.3, 0.02)]), Mat.alu, 0, bottomY, 0);

        // Standoffs with screw heads on both plates.
        const standoffLength = topY - bottomY - T;
        const standoffs: [number, number][] = [[0.5, 0.3], [0.5, -0.3], [-0.5, 0.3], [-0.5, -0.3], [0, 0.28], [0, -0.28]];
        for (const [sx, sz] of standoffs) {
            b.cyl(0.03, standoffLength, Mat.brass, sx, (topY + bottomY) / 2, sz, 'y', 6);
            b.cyl(0.028, 0.012, Mat.steel, sx, topY + T / 2 + 0.006, sz, 'y', 12);
            b.cyl(0.028, 0.012, Mat.steel, sx, bottomY - T / 2 - 0.006, sz, 'y', 12);
        }

        // Horn screws around every coxa mount.
        for (const [mx, mz] of mounts) {
            for (let i = 0; i < 4; i++) {
                const a = Math.PI / 4 + (Math.PI / 2) * i;
                b.cyl(0.011, 0.008, Mat.steel, mx + Math.cos(a) * 0.085, topY + T / 2 + 0.004, mz + Math.sin(a) * 0.085, 'y', 8);
            }
        }

        // Push button (BP) and status LEDs on the top plate.
        b.cyl(0.04, 0.04, Mat.steel, -0.72, topY + T / 2 + 0.01, 0, 'y', 20);
        b.cyl(0.028, 0.03, Mat.ledCyan, -0.72, topY + T / 2 + 0.045, 0, 'y', 20);
        b.cyl(0.012, 0.02, Mat.ledRed, 0.62, topY + T / 2 + 0.01, -0.12, 'y', 10);
        b.cyl(0.012, 0.02, Mat.ledGreen, 0.62, topY + T / 2 + 0.01, 0.12, 'y', 10);

        this.addElectronics(b, topY - T / 2);
        this.addBattery(b, bottomY - T / 2);
        b.build(this);
    }

    // Boards hang under the top plate, components facing up so the cut-out shows them.
    private addElectronics(b: ModelBuilder, plateUnderside: number) {
        const standoff = (x: number, y: number, z: number, length: number) =>
            b.cyl(0.014, length, Mat.nylon, x, y, z, 'y', 8);

        // Raspberry Pi Zero 2 W
        const piY = plateUnderside - 0.07;
        b.box(0.65, 0.016, 0.3, Mat.pcbGreen, 0, piY, 0);
        b.box(0.13, 0.022, 0.13, Mat.silver, -0.08, piY + 0.019, 0);
        b.box(0.5, 0.03, 0.05, Mat.black, 0.02, piY + 0.023, -0.1);
        b.box(0.1, 0.022, 0.12, Mat.silver, 0.27, piY + 0.019, 0);
        b.box(0.06, 0.02, 0.05, Mat.silver, -0.2, piY + 0.018, 0.14);
        b.box(0.06, 0.02, 0.05, Mat.silver, -0.1, piY + 0.018, 0.14);
        for (const sx of [-0.3, 0.3]) {
            for (const sz of [-0.12, 0.12]) {
                standoff(sx, piY + 0.04, sz, 0.065);
            }
        }

        // Two PCA9685 servo drivers stacked under the Pi
        for (const [i, color] of [[0, Mat.pcbBlue], [1, Mat.pcbBlue]] as const) {
            const py = piY - 0.075 * (i + 1);
            b.box(0.625, 0.016, 0.254, color, 0, py, 0);
            b.box(0.12, 0.05, 0.07, Mat.terminal, -0.25, py + 0.033, 0.07);
            b.box(0.52, 0.03, 0.06, Mat.black, 0.03, py + 0.023, -0.09);
            b.box(0.05, 0.018, 0.05, Mat.black, 0.22, py + 0.017, 0.07);
            for (const sx of [-0.28, 0.28]) {
                standoff(sx, py + 0.037, 0.1, 0.06);
            }
        }

        // IMU / barometer breakout
        b.box(0.2, 0.012, 0.13, Mat.pcbPurple, 0.38, piY + 0.04, 0.02);
        b.box(0.06, 0.016, 0.06, Mat.black, 0.38, piY + 0.054, 0.02);
        standoff(0.38, piY + 0.075, 0.02, 0.03);
    }

    private addBattery(b: ModelBuilder, plateBottom: number) {
        const bh = 0.19;
        const cy = plateBottom - bh / 2;
        b.box(0.95, bh, 0.34, Mat.battery, 0, cy, 0);
        b.box(0.5, bh + 0.002, 0.2, Mat.pcbBlue, -0.05, cy, 0);
        for (const sx of [-0.25, 0.25]) {
            b.box(0.06, bh + 0.02, 0.36, Mat.strap, sx, cy, 0);
        }
        b.box(0.05, 0.05, 0.07, Mat.xt60, -0.5, cy, 0);
        b.cyl(0.012, 0.18, Mat.wireRed, -0.5, cy + 0.04, -0.02, 'x', 8);
        b.cyl(0.012, 0.18, Mat.black, -0.5, cy + 0.04, 0.02, 'x', 8);
    }

    update(dt: number = 1 / 60) {
        this.members?.update(dt);
    }

    setDirection(direction: number) {
        this.members?.setDirection(direction);
    }
}

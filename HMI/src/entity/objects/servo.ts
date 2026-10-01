import { Object3D } from 'three'
import Socket from "../../communication/socket.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterServoCommands} from "../../communication/clusters/clusterServo.ts";
import Message from "../../communication/message.ts";
import { Mat, ModelBuilder } from "./materials.ts";

export interface ServoStruct {
    name: string;
    id: number;
    angle: number;
    offset: number;
    status: boolean;
    min: number;
    max: number;
    reversed: boolean;
}

export default class Servo extends Object3D {
    x: number;
    y: number;
    z: number;
    readonly width: number = 0.09;
    servoData: ServoStruct;
    socket: Socket;
    private shownAngle = Number.NaN;

    constructor(name: string, id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.x = x;
        this.y = y;
        this.z = z;
        this.socket = socket;
        this.servoData = {name: name, angle: 90, offset: 0, status: false, id: id, min: 0, max: 180, reversed: false};

        this.buildModel();
        this.position.set(x, y, z);
        this.addServoCallbacks();
    }

    // Hitec HS-645MG. Model frame: output shaft on +Y, case length along +X (shaft 1 cm from the end),
    // width along Z, origin on the shaft axis at the top face of the case.
    private buildModel() {
        const L = 0.406;
        const W = 0.198;
        const shaftOffset = 0.1;
        const cx = -shaftOffset + L / 2;
        const b = new ModelBuilder();

        b.box(L, 0.326, W, Mat.servo, cx, -0.203, 0);
        b.cyl(0.099, 0.04, Mat.servoCap, 0, -0.02, 0, 'y', 28);
        b.box(0.2, 0.04, W, Mat.servoCap, 0.1, -0.02, 0);
        b.cyl(0.05, 0.012, Mat.steel, 0, 0.006, 0, 'y', 16);
        b.cyl(0.118, 0.016, Mat.horn, 0, 0.02, 0, 'y', 36);
        b.cyl(0.026, 0.01, Mat.steel, 0, 0.033, 0, 'y', 12);

        b.box(0.548, 0.026, W, Mat.servo, cx, -0.12, 0);
        for (const sx of [-1, 1]) {
            for (const sz of [-1, 1]) {
                b.cyl(0.014, 0.008, Mat.steel, cx + sx * 0.235, -0.103, sz * 0.07, 'y', 10);
            }
        }

        b.box(0.05, 0.08, 0.11, Mat.black, cx + L / 2 + 0.02, -0.3, 0);
        b.plane(0.26, 0.1, Mat.servoLabel, cx, -0.25, W / 2 + 0.001);
        b.plane(0.26, 0.1, Mat.servoLabel, cx, -0.25, -W / 2 - 0.001, true);

        b.build(this);
    }

    addServoCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_SERVO_ALL, (message: Message) => {
            if (message.getValueUint8(0) == this.servoData.id) {
                this.servoData.status = message.getValueUint8(1) !== 0;
                this.servoData.angle = message.getValueUint8(2);
                this.servoData.offset = message.getValueUint8(3);
                this.servoData.min = message.getValueUint8(4);
                this.servoData.max = message.getValueUint8(5);
                this.servoData.reversed = message.getValueUint8(6) !== 0;
            }
        }, [this.servoData.id]);
    }

    setStatus(status: boolean) {
        this.servoData.status = status;
    }

    getStatus(): boolean {
        return this.servoData.status;
    }

    setAngle(angle: number) {
        if (this.getStatus()) {
            this.servoData.angle = (((angle - 90) * -1) + 90);
        }
    }

    getAngle(): number {
        return  (((this.servoData.angle - 90) * -1) + 90);
    }

    // Angles arrive a few times per second; ease toward them so the legs move continuously.
    smooth(dt: number): number {
        const target = this.getAngle();
        this.shownAngle = Number.isNaN(this.shownAngle)
            ? target
            : this.shownAngle + (target - this.shownAngle) * (1 - Math.exp(-dt * 18));
        return this.shownAngle;
    }
}

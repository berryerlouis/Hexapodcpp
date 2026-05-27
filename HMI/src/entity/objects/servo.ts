import {Mesh, MeshPhongMaterial, Object3D, SphereGeometry} from 'three'
import Socket from "../../communication/socket.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterServoCommands} from "../../communication/clusters/clusterServo.ts";
import Message from "../../communication/message.ts";

export interface ServoStruct {
    name: string;
    id: number;
    angle: number;
    status: boolean;
    min: number;
    max: number;
}

export default class Servo extends Object3D {
    x: number;
    y: number;
    z: number;
    readonly width: number = 0.09;
    servoData: ServoStruct;
    socket: Socket;

    constructor(name: string, id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.x = x;
        this.y = y;
        this.z = z;
        this.socket = socket;
        this.servoData = {name: name, angle: 90, status: false, id: id, min: 0, max: 180};
        const geometry = new SphereGeometry(this.width, 10, 10);
        const material = new MeshPhongMaterial({color: '#909090'});
        const body = new Mesh(geometry, material);
        this.position.set(x, y, z);
        this.addServoCallbacks();

        this.add(body);
    }

    addServoCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_ANGLE, (message: Message) => {
            if (message.getValueUint8(0) == this.servoData.id) {
                this.servoData.angle = message.getValueUint8(1);
            }
        }, [this.servoData.id]);
        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_MIN, (message: Message) => {
            if (message.getValueUint8(0) == this.servoData.id) {
                this.servoData.min = message.getValueUint8(1);
            }
        }, [this.servoData.id]);
        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_MAX, (message: Message) => {
            if (message.getValueUint8(0) == this.servoData.id) {
                this.servoData.max = message.getValueUint8(1);
            }
        }, [this.servoData.id]);
        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_STATE, (message: Message) => {
            if (message.getValueUint8(0) == this.servoData.id) {
                this.servoData.status = message.getValueBool(1);
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
}
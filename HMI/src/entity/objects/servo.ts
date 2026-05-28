import {BoxGeometry, CylinderGeometry, Mesh, MeshPhongMaterial, Object3D} from 'three'
import Socket from "../../communication/socket.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterServoCommands} from "../../communication/clusters/clusterServo.ts";
import Message from "../../communication/message.ts";

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

    constructor(name: string, id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.x = x;
        this.y = y;
        this.z = z;
        this.socket = socket;
        this.servoData = {name: name, angle: 90, status: false, id: id, min: 0, max: 180};

        // Hitec HS-645MG style standard servo (Capers II uses these).
        // Dimensions roughly mirror the real ~40 x 20 x 38 mm body, scaled to scene units.
        const caseW = 0.18; // length along X (long side)
        const caseD = 0.09; // depth along Z (short side)
        const caseH = 0.20; // total height along Y
        const caseMat = new MeshPhongMaterial({color: '#1a1a1a', shininess: 30});
        const caseGeom = new BoxGeometry(caseW, caseH, caseD);
        const body = new Mesh(caseGeom, caseMat);
        this.add(body);

        // Top gear housing — small raised box on the horn side
        const gearHousingMat = new MeshPhongMaterial({color: '#2a2a2a', shininess: 20});
        const gearHousing = new Mesh(
            new BoxGeometry(caseW * 0.55, 0.035, caseD),
            gearHousingMat
        );
        gearHousing.position.set(-caseW * 0.225, caseH / 2 + 0.0175, 0);
        this.add(gearHousing);

        // Mounting tabs / flanges sticking out on each long side
        const tabMat = new MeshPhongMaterial({color: '#1a1a1a', shininess: 30});
        const tab = new Mesh(
            new BoxGeometry(caseW * 1.5, 0.012, caseD),
            tabMat
        );
        tab.position.set(0, caseH * 0.18, 0);
        this.add(tab);

        // Servo horn / wheel — white circular disc on top of the gear shaft.
        // This is the "wheel" that bolts to the next bracket.
        const hornMat = new MeshPhongMaterial({color: '#f2f2f2', shininess: 80});
        const horn = new Mesh(
            new CylinderGeometry(0.075, 0.075, 0.018, 28),
            hornMat
        );
        horn.position.set(-caseW * 0.225, caseH / 2 + 0.035 + 0.009, 0);
        this.add(horn);

        // Horn center hub (slightly raised) + retaining screw
        const hubMat = new MeshPhongMaterial({color: '#d0d0d0', shininess: 60});
        const hub = new Mesh(
            new CylinderGeometry(0.022, 0.022, 0.012, 16),
            hubMat
        );
        hub.position.set(-caseW * 0.225, caseH / 2 + 0.035 + 0.018 + 0.006, 0);
        this.add(hub);

        this.position.set(x, y, z);
        this.addServoCallbacks();
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
}
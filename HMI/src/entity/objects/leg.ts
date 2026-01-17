import {CircleGeometry, DoubleSide, MathUtils, Mesh, MeshBasicMaterial, Object3D, RingGeometry} from 'three'
import Coxys from "./coxys.ts";
import {ServoStruct} from "./servo.ts";
import Socket from "../../communication/socket.ts";

export interface LegStruct {
    name: string;
    servos: ServoStruct[];
}

export default class Leg extends Object3D {
    coxys: Coxys;
    x: number;
    y: number;
    z: number;
    legData: LegStruct;
    socket: Socket;
    direction: Mesh;

    constructor(name: string, id: number, x: number, y: number, z: number, leftSide: boolean, socket: Socket) {
        super()
        this.x = x;
        this.y = y;
        this.z = z;
        this.socket = socket;

        this.coxys = new Coxys(id * 3, this.x, this.y, this.z, this.socket);
        this.coxys.rotation.set(MathUtils.degToRad(90), 0, leftSide ? MathUtils.degToRad(180) : 0);
        this.add(this.coxys);

        const geometry = new RingGeometry(0.25, 0.3, 32);
        geometry.rotateX(Math.PI / 2);
        geometry.rotateY(Math.PI);
        geometry.translate(this.x, 0.001, this.z > 0 ? this.z + 1 : this.z - 1);
        const material = new MeshBasicMaterial({color: 0x4a6a8f});
        material.side = DoubleSide;
        const circle = new Mesh(geometry, material);
        circle.rotateY(0);
        this.add(circle);

        const geometry1 = new CircleGeometry(0.05, 32);
        geometry1.rotateX(Math.PI / 2);
        geometry1.translate(0.275, 0, 0);
        const material1 = new MeshBasicMaterial({color: 0x192835});
        material1.side = DoubleSide;
        this.direction = new Mesh(geometry1, material1);
        this.direction.translateX(this.x);
        this.direction.translateY(0.0015);
        this.direction.translateZ(this.z > 0 ? this.z + 1 : this.z - 1);
        this.add(this.direction);

        this.legData = {
            name: name,
            servos: [this.coxys.servo.servoData,
                this.coxys.femur.servo.servoData,
                this.coxys.femur.tibia.servo.servoData]
        };
    }

    setDirection(direction: number) {
        this.direction.rotation.y = (MathUtils.degToRad(direction));
    }

    setCoxysAngle(angle: number) {
        this.coxys.setAngle(angle);
    }

    setFemurAngle(angle: number) {
        this.coxys.femur.setAngle(angle);
    }

    setTibiaAngle(angle: number) {
        this.coxys.femur.tibia.setAngle(angle);
    }

    update() {
        this.coxys.update();
        this.coxys.femur.update();
        this.coxys.femur.tibia.update();
    }
}
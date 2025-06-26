import {ConeGeometry, MathUtils, Mesh, MeshPhongMaterial, Object3D} from 'three'
import Tibia from "./tibia.ts";
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";

export default class Femur extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = 0.7;
    femurBody: Mesh;
    tibia: Tibia;
    servo: Servo;
    angle: number = 0;
    socket: Socket;

    constructor(id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;
        const geometry = new ConeGeometry(this.width, this.height, 3, 3);
        const material = new MeshPhongMaterial({color: '#605800'});
        this.femurBody = new Mesh(geometry, material);
        this.position.set(x, y, z);
        this.femurBody.geometry.translate(0, this.height / 2, 0);

        this.servo = new Servo('femur', id, 0, this.height, 0, this.socket);
        this.tibia = new Tibia(id + 1, 0, this.height, 0, this.socket);
        this.femurBody.add(this.servo);
        this.femurBody.add(this.tibia);
        this.add(this.femurBody);
    }

    setAngle(angle: number) {
        this.servo.setAngle(angle);
    }

    update() {
        this.femurBody.rotation.x = MathUtils.degToRad(this.servo.getAngle() - 90);
    }
}
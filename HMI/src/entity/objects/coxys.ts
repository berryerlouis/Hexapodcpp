import { BoxGeometry, CylinderGeometry, MathUtils, Mesh, MeshStandardMaterial, Object3D } from 'three'
import Femur from "./femur.ts";
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";

export default class Coxys extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = 0.3;
    coxysBody: Mesh;
    femur: Femur;
    servo: Servo;
    socket: Socket;

    constructor(id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;

        // Servo housing — rectangular box like a real servo body
        const servoMat = new MeshStandardMaterial({ color: '#b0b0b0', metalness: 0.5, roughness: 0.5 });
        const servoGeom = new BoxGeometry(0.20, this.height, 0.24);
        this.coxysBody = new Mesh(servoGeom, servoMat);
        this.position.set(x, y, z);
        this.coxysBody.geometry.translate(0, this.height / 2, 0);

        // Bottom mount flange (body-side)
        const flangeMat = new MeshStandardMaterial({ color: '#e0e0e0', metalness: 0.85, roughness: 0.2 });
        const bottomFlangeGeom = new CylinderGeometry(0.13, 0.13, 0.03, 12);
        const bottomFlange = new Mesh(bottomFlangeGeom, flangeMat);
        bottomFlange.position.set(0, 0, 0);
        this.coxysBody.add(bottomFlange);

        // Top output flange (femur-side)
        const topFlangeGeom = new CylinderGeometry(0.10, 0.10, 0.04, 12);
        const topFlange = new Mesh(topFlangeGeom, flangeMat);
        topFlange.position.set(0, this.height, 0);
        this.coxysBody.add(topFlange);

        // Servo label stripe
        const stripeMat = new MeshStandardMaterial({ color: '#707070', metalness: 0.2, roughness: 0.8 });
        const stripeGeom = new BoxGeometry(0.21, 0.03, 0.05);
        const stripe = new Mesh(stripeGeom, stripeMat);
        stripe.position.set(0, this.height * 0.65, 0.115);
        this.coxysBody.add(stripe);

        this.servo = new Servo('coxys', id, 0, this.height, 0, this.socket);
        this.femur = new Femur(id + 1, 0, this.height, 0, this.socket);
        this.coxysBody.add(this.servo);
        this.coxysBody.add(this.femur);

        this.add(this.coxysBody);
    }

    setAngle(angle: number) {
        this.servo.setAngle(angle);
    }

    update() {
        this.coxysBody.rotation.z = MathUtils.degToRad(this.servo.getAngle() - 90);
    }
}
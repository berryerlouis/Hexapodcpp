import { CylinderGeometry, MathUtils, Mesh, MeshStandardMaterial, Object3D } from 'three'
import Femur from "./femur.ts";
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";

export default class Coxys extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = 0.28;
    coxysBody: Mesh;
    femur: Femur;
    servo: Servo;
    socket: Socket;

    constructor(id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;

        // Servo housing — rounded capsule shape for smooth Cappers II look
        const servoMat = new MeshStandardMaterial({ color: '#b0b0b0', metalness: 0.5, roughness: 0.5 });
        const servoGeom = new CylinderGeometry(0.105, 0.105, this.height, 14, 4);
        this.coxysBody = new Mesh(servoGeom, servoMat);
        this.position.set(x, y, z);
        this.coxysBody.geometry.translate(0, this.height / 2, 0);

        // Bottom mount flange (body-side) — high-poly for smoothness
        const flangeMat = new MeshStandardMaterial({ color: '#e0e0e0', metalness: 0.85, roughness: 0.2 });
        const bottomFlangeGeom = new CylinderGeometry(0.13, 0.13, 0.03, 16, 2);
        const bottomFlange = new Mesh(bottomFlangeGeom, flangeMat);
        bottomFlange.position.set(0, 0, 0);
        this.coxysBody.add(bottomFlange);

        // Top output flange (femur-side) — high-poly cylinder
        const topFlangeGeom = new CylinderGeometry(0.10, 0.10, 0.04, 16, 2);
        const topFlange = new Mesh(topFlangeGeom, flangeMat);
        topFlange.position.set(0, this.height, 0);
        this.coxysBody.add(topFlange);

        // Servo label stripe — rounded cylinder instead of box
        const stripeMat = new MeshStandardMaterial({ color: '#707070', metalness: 0.2, roughness: 0.8 });
        const stripeGeom = new CylinderGeometry(0.085, 0.085, 0.05, 12, 2);
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
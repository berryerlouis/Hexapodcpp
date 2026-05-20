import { CylinderGeometry, MathUtils, Mesh, MeshStandardMaterial, Object3D } from 'three'
import Tibia from "./tibia.ts";
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";

export default class Femur extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = 0.78;
    femurBody: Mesh;
    tibia: Tibia;
    servo: Servo;
    angle: number = 0;
    socket: Socket;

    constructor(id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;

        // Main arm bar — tapered cylinder for smooth, rounded appearance
        const armMat = new MeshStandardMaterial({ color: '#b8b8b8', metalness: 0.5, roughness: 0.45 });
        const armGeom = new CylinderGeometry(0.055, 0.045, this.height, 12, 8);
        this.femurBody = new Mesh(armGeom, armMat);
        this.position.set(x, y, z);
        this.femurBody.geometry.translate(0, this.height / 2, 0);

        // Pivot joint at base (coxa output, joint axis along Z) — high-poly for smoothness
        const jointMat = new MeshStandardMaterial({ color: '#e0e0e0', metalness: 0.85, roughness: 0.2 });
        const baseJointGeom = new CylinderGeometry(0.09, 0.09, 0.20, 16, 4);
        const baseJoint = new Mesh(baseJointGeom, jointMat);
        baseJoint.rotation.x = Math.PI / 2;
        baseJoint.position.set(0, 0, 0);
        this.femurBody.add(baseJoint);

        // Servo housing at distal end — rounded capsule shape
        const servoMat = new MeshStandardMaterial({ color: '#b0b0b0', metalness: 0.5, roughness: 0.5 });
        const servoGeom = new CylinderGeometry(0.095, 0.095, 0.18, 14, 4);
        const servoHousing = new Mesh(servoGeom, servoMat);
        servoHousing.position.set(0, this.height, 0);
        this.femurBody.add(servoHousing);

        // Output flange at distal end (tibia side) — high-poly cylinder
        const topFlangeGeom = new CylinderGeometry(0.10, 0.10, 0.04, 16, 2);
        const topFlange = new Mesh(topFlangeGeom, jointMat);
        topFlange.rotation.x = Math.PI / 2;
        topFlange.position.set(0, this.height, 0);
        this.femurBody.add(topFlange);

        // Servo label stripe — rounded edges
        const stripeMat = new MeshStandardMaterial({ color: '#707070', metalness: 0.2, roughness: 0.8 });
        const stripeGeom = new CylinderGeometry(0.08, 0.08, 0.05, 12, 2);
        const stripe = new Mesh(stripeGeom, stripeMat);
        stripe.position.set(0, this.height * 0.65, 0.08);
        this.femurBody.add(stripe);

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
import {
    CylinderGeometry,
    MathUtils,
    Mesh,
    MeshStandardMaterial,
    Object3D,
    SphereGeometry
} from 'three'
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";

export default class Tibia extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = 1.12;
    tibiaBody: Mesh;
    servo: Servo;
    angle: number = 0;
    socket: Socket;

    constructor(id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;

        // Tapered tube — smooth, high-poly for realistic appearance
        const tubeMat = new MeshStandardMaterial({ color: '#c0c0c0', metalness: 0.5, roughness: 0.45 });
        const tubeGeom = new CylinderGeometry(0.04, 0.08, this.height, 14, 8);
        this.tibiaBody = new Mesh(tubeGeom, tubeMat);
        this.position.set(x, y, z);
        this.tibiaBody.geometry.translate(0, this.height / 2, 0);
        this.rotation.set(MathUtils.degToRad(90), 0, 0);

        // Pivot joint at base (femur output, joint axis along Z) — high-poly smooth joint
        const jointMat = new MeshStandardMaterial({ color: '#e0e0e0', metalness: 0.85, roughness: 0.2 });
        const pivotGeom = new CylinderGeometry(0.09, 0.09, 0.20, 16, 4);
        const pivot = new Mesh(pivotGeom, jointMat);
        pivot.rotation.x = Math.PI / 2;
        pivot.position.set(0, 0, 0);
        this.tibiaBody.add(pivot);

        // Rubber foot tip at distal end — enhanced rounded capper
        const footMat = new MeshStandardMaterial({ color: '#555555', metalness: 0.0, roughness: 0.95 });
        // High-poly sphere for smooth, realistic rounded shape (24 width segments, 16 height segments)
        const footGeom = new SphereGeometry(0.055, 24, 16);
        const foot = new Mesh(footGeom, footMat);
        foot.position.set(0, this.height, 0);
        this.tibiaBody.add(foot);

        this.servo = new Servo('tibia', id, 0, 0, 0, this.socket);

        this.add(this.tibiaBody);
    }

    setAngle(angle: number) {
        this.servo.setAngle(angle);
    }

    update() {
        this.tibiaBody.rotation.x = MathUtils.degToRad(this.servo.getAngle() - 90);
    }
}
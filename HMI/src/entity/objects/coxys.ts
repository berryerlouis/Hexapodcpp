import { BoxGeometry, CylinderGeometry, MathUtils, Mesh, MeshStandardMaterial, Object3D } from 'three'
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

        // Capers II coxa is a servo-bracket assembly: two flat aluminium side plates
        // joined by a base plate, with a ball-bearing housing at the top.
        const aluMat = new MeshStandardMaterial({ color: '#c8c8cc', metalness: 0.7, roughness: 0.35 });
        const screwMat = new MeshStandardMaterial({ color: '#303030', metalness: 0.6, roughness: 0.4 });

        // Root mesh = base plate (lying flat at the body-side mount).
        const basePlateGeom = new BoxGeometry(0.32, 0.025, 0.22);
        this.coxysBody = new Mesh(basePlateGeom, aluMat);
        this.position.set(x, y, z);
        this.coxysBody.geometry.translate(0, 0.0125, 0);

        // Two upright side plates (left/right along X), forming the bracket walls.
        const sidePlateGeom = new BoxGeometry(0.025, this.height - 0.025, 0.22);
        const leftPlate = new Mesh(sidePlateGeom, aluMat);
        leftPlate.position.set(-0.145, 0.025 + (this.height - 0.025) / 2, 0);
        this.coxysBody.add(leftPlate);
        const rightPlate = new Mesh(sidePlateGeom, aluMat);
        rightPlate.position.set(0.145, 0.025 + (this.height - 0.025) / 2, 0);
        this.coxysBody.add(rightPlate);

        // Ball-bearing housing at top of one side plate (Capers II uses a flanged bearing).
        const bearingHousing = new Mesh(
            new CylinderGeometry(0.06, 0.06, 0.028, 20),
            aluMat
        );
        bearingHousing.rotation.z = Math.PI / 2;
        bearingHousing.position.set(-0.145, this.height - 0.04, 0);
        this.coxysBody.add(bearingHousing);

        // Inner bearing race (darker)
        const bearingInner = new Mesh(
            new CylinderGeometry(0.022, 0.022, 0.032, 16),
            screwMat
        );
        bearingInner.rotation.z = Math.PI / 2;
        bearingInner.position.set(-0.145, this.height - 0.04, 0);
        this.coxysBody.add(bearingInner);

        // The coxa servo sits inside the bracket, horn pointing up toward the femur joint.
        // Visual servo body is the Servo instance below; placed centered between the plates.
        this.servo = new Servo('coxys', id, 0, this.height - 0.16, 0, this.socket);
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
import {
    BoxGeometry,
    CylinderGeometry,
    ExtrudeGeometry,
    MathUtils,
    Mesh,
    MeshStandardMaterial,
    Object3D,
    Shape,
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

        // Capers II tibia is a single flat aluminium bracket, wide at the femur/servo
        // mount end and tapering smoothly to a point at the foot, with a rubber tip
        // bonded onto the end. The bracket is flat in the rotation plane.
        const aluMat = new MeshStandardMaterial({ color: '#c8c8cc', metalness: 0.7, roughness: 0.35 });
        const hornMat = new MeshStandardMaterial({ color: '#f2f2f2', metalness: 0.05, roughness: 0.55 });
        const footMat = new MeshStandardMaterial({ color: '#202020', metalness: 0.0, roughness: 0.95 });

        // Build the tapered flat-plate shape in 2D (x = length along tibia, y = width).
        const L = this.height;
        const halfTop = 0.085;   // wide end at femur side
        const halfBot = 0.018;   // narrow end at foot
        const plateThk = 0.022;
        const shape = new Shape();
        shape.moveTo(0, -halfTop);
        shape.lineTo(0, halfTop);
        shape.lineTo(L, halfBot);
        shape.lineTo(L, -halfBot);
        shape.closePath();
        const plateGeom = new ExtrudeGeometry(shape, { depth: plateThk, bevelEnabled: false });
        // Centre the plate thickness around X=0 then re-orient axes so that:
        //   shape "length" (X) -> local Y (along the tibia)
        //   shape "width"  (Y) -> local Z (front-back)
        //   extrude depth  (Z) -> local X (thickness)
        plateGeom.translate(0, 0, -plateThk / 2);
        plateGeom.rotateZ(Math.PI / 2);   // length axis: world X -> world Y
        plateGeom.rotateY(Math.PI / 2);   // depth axis: world Z -> world X

        this.tibiaBody = new Mesh(plateGeom, aluMat);
        this.position.set(x, y, z);
        this.rotation.set(MathUtils.degToRad(90), 0, 0);

        // Reinforcement boss at the femur-servo mount (wide end).
        const mountBoss = new Mesh(
            new BoxGeometry(plateThk + 0.012, 0.14, halfTop * 1.6),
            aluMat
        );
        mountBoss.position.set(0, 0.07, 0);
        this.tibiaBody.add(mountBoss);

        // White servo horns on each X-side at the femur pivot.
        const horn = new Mesh(
            new CylinderGeometry(0.07, 0.07, 0.012, 28),
            hornMat
        );
        horn.rotation.z = Math.PI / 2;
        horn.position.set(plateThk / 2 + 0.012, 0, 0);
        this.tibiaBody.add(horn);
        const hornB = horn.clone();
        hornB.position.set(-plateThk / 2 - 0.012, 0, 0);
        this.tibiaBody.add(hornB);

        // Rubber foot tip at the distal end.
        const foot = new Mesh(new SphereGeometry(0.045, 20, 14), footMat);
        foot.position.set(0, this.height, 0);
        this.tibiaBody.add(foot);

        this.servo = new Servo('tibia', id, 0, 0.04, 0, this.socket);

        this.add(this.tibiaBody);
    }

    setAngle(angle: number) {
        this.servo.setAngle(angle);
    }

    update() {
        this.tibiaBody.rotation.x = MathUtils.degToRad(this.servo.getAngle() - 90);
    }
}
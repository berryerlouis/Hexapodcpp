import { BoxGeometry, CylinderGeometry, MathUtils, Mesh, MeshStandardMaterial, Object3D } from 'three'
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

        // Capers II femur is a flat aluminium "U" bracket: two side plates joined by
        // a top strap, with white servo horns bolted to both side plates at the
        // coxa-side pivot. The femur rotates around the X axis at its base.
        const aluMat = new MeshStandardMaterial({ color: '#c8c8cc', metalness: 0.7, roughness: 0.35 });
        const hornMat = new MeshStandardMaterial({ color: '#f2f2f2', metalness: 0.05, roughness: 0.55 });
        const hubMat = new MeshStandardMaterial({ color: '#404040', metalness: 0.5, roughness: 0.5 });

        const gap = 0.20;            // distance between the two plates (X)
        const plateThk = 0.022;      // plate thickness (X)
        const plateDepth = 0.14;     // plate depth (Z, front-back)
        const plateLen = this.height; // plate length (Y)

        // Root mesh = cross strap at the base (pivot connector, also visible).
        const baseStrapGeom = new BoxGeometry(gap, 0.022, plateDepth * 0.6);
        this.femurBody = new Mesh(baseStrapGeom, aluMat);
        this.position.set(x, y, z);

        // Two side plates running up along Y.
        const plateGeom = new BoxGeometry(plateThk, plateLen, plateDepth);
        const leftPlate = new Mesh(plateGeom, aluMat);
        leftPlate.position.set(-gap / 2 - plateThk / 2, plateLen / 2, 0);
        this.femurBody.add(leftPlate);
        const rightPlate = new Mesh(plateGeom, aluMat);
        rightPlate.position.set(gap / 2 + plateThk / 2, plateLen / 2, 0);
        this.femurBody.add(rightPlate);

        // Top connecting strap closing the U.
        const topStrap = new Mesh(
            new BoxGeometry(gap + 2 * plateThk, 0.025, plateDepth),
            aluMat
        );
        topStrap.position.set(0, plateLen - 0.0125, 0);
        this.femurBody.add(topStrap);

        // White circular servo horns on each X side at the base pivot (coxa side).
        const hornGeom = new CylinderGeometry(0.07, 0.07, 0.012, 28);
        const baseHornL = new Mesh(hornGeom, hornMat);
        baseHornL.rotation.z = Math.PI / 2;
        baseHornL.position.set(-gap / 2 - plateThk - 0.007, 0, 0);
        this.femurBody.add(baseHornL);
        const baseHornR = new Mesh(hornGeom, hornMat);
        baseHornR.rotation.z = -Math.PI / 2;
        baseHornR.position.set(gap / 2 + plateThk + 0.007, 0, 0);
        this.femurBody.add(baseHornR);

        // Small hub caps (screws) at the horn centers.
        const hubGeom = new CylinderGeometry(0.018, 0.018, 0.018, 12);
        const hubL = new Mesh(hubGeom, hubMat);
        hubL.rotation.z = Math.PI / 2;
        hubL.position.set(-gap / 2 - plateThk - 0.018, 0, 0);
        this.femurBody.add(hubL);
        const hubR = new Mesh(hubGeom, hubMat);
        hubR.rotation.z = -Math.PI / 2;
        hubR.position.set(gap / 2 + plateThk + 0.018, 0, 0);
        this.femurBody.add(hubR);

        // The tibia servo lives between the plates at the distal end; the Servo
        // instance below will render the actual HS-645MG body there.
        this.servo = new Servo('femur', id, 0, this.height - 0.10, 0, this.socket);
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
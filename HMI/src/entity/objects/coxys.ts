import { Group, MathUtils, Object3D, Vector3 } from 'three'
import Femur from "./femur.ts";
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";
import { LEG, Mat, ModelBuilder, orient } from "./materials.ts";

// Frame: +Y points away from the body, +Z points down, +X runs along the body (forward on the right side).
export default class Coxys extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = LEG.coxa;
    coxysBody: Group;
    femur: Femur;
    servo: Servo;
    socket: Socket;

    constructor(id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;
        this.position.set(x, y, z);
        this.coxysBody = new Group();

        // Coxa servo: vertical shaft on the leg axis, case pointing inboard, horn bolted to the top plate.
        this.servo = new Servo('coxys', id, 0, 0, -0.25, this.socket);
        orient(this.servo, new Vector3(0, -1, 0), new Vector3(0, 0, -1), new Vector3(1, 0, 0));
        this.coxysBody.add(this.servo);

        this.femur = new Femur(id + 1, 0, this.height, 0, this.socket);
        // Femur servo sits in the C-bracket, shaft along +X, case pointing outward.
        const femurServo = this.femur.servo;
        orient(femurServo, new Vector3(0, 1, 0), new Vector3(1, 0, 0), new Vector3(0, 0, -1));
        femurServo.position.set(0, this.height, 0);
        this.coxysBody.add(femurServo);

        const b = new ModelBuilder();
        const y0 = 0.04;
        const y1 = 0.62;
        const len = y1 - y0;
        const yc = (y0 + y1) / 2;
        b.box(0.405, len, 0.025, Mat.alu, -0.1825, yc, -0.1155);
        b.box(0.405, len, 0.025, Mat.alu, -0.1825, yc, 0.1155);
        b.box(0.025, len, 0.256, Mat.alu, -0.3975, yc, 0);
        for (const sx of [-0.33, -0.12]) {
            for (const sy of [0.5, 0.58]) {
                b.cyl(0.014, 0.008, Mat.steel, sx, sy, -0.1355, 'z', 10);
            }
        }

        // Flanged ball bearing under the coxa servo shaft, riding in the lower plate.
        b.cyl(0.04, 0.084, Mat.steel, 0, 0, 0.158, 'z', 16);
        b.cyl(0.065, 0.012, Mat.silver, 0, 0, 0.194, 'z', 28);
        b.cyl(0.05, 0.012, Mat.silver, 0, 0, 0.236, 'z', 24);
        b.build(this.coxysBody);

        this.coxysBody.add(this.femur);
        this.add(this.coxysBody);
    }

    setAngle(angle: number) {
        this.servo.setAngle(angle);
    }

    update(dt: number = 1 / 60) {
        this.coxysBody.rotation.z = MathUtils.degToRad(this.servo.smooth(dt) - 90);
    }
}

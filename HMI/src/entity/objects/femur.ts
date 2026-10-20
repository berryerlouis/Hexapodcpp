import { ExtrudeGeometry, Group, MathUtils, Matrix4, Object3D, Path, Shape, Vector2, Vector3 } from 'three'
import Tibia from "./tibia.ts";
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";
import { LEG, Mat, ModelBuilder } from "./materials.ts";

// Frame: origin on the femur axis (along X), +Y runs along the femur, +Z points down.
// The femur is a flat bracket bolted to the femur servo horn; the tibia servo is carried by the tibia.
export default class Femur extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = LEG.femur;
    femurBody: Group;
    tibia: Tibia;
    servo: Servo;
    angle: number = 0;
    socket: Socket;

    constructor(id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;
        this.position.set(x, y, z);

        this.servo = new Servo('femur', id, 0, 0, 0, this.socket);
        this.tibia = new Tibia(id + 1, -0.085, this.height, 0, this.socket);
        this.femurBody = new Group();

        const L = this.height;
        const R0 = 0.125;
        const R1 = 0.115;
        const outline: Vector2[] = [];
        for (let i = 0; i <= 16; i++) {
            const t = Math.PI / 2 + (Math.PI * i) / 16;
            outline.push(new Vector2(R0 * Math.cos(t), R0 * Math.sin(t)));
        }
        for (let i = 0; i <= 16; i++) {
            const t = -Math.PI / 2 + (Math.PI * i) / 16;
            outline.push(new Vector2(L + R1 * Math.cos(t), R1 * Math.sin(t)));
        }
        const shape = new Shape(outline);
        for (const u of [0.3, 0.46, 0.62]) {
            const hole = new Path();
            hole.absarc(u, 0, 0.04, 0, Math.PI * 2, true);
            shape.holes.push(hole);
        }

        const thickness = 0.025;
        const plate = new ExtrudeGeometry(shape, { depth: thickness, bevelEnabled: false, curveSegments: 16 });
        // Shape (u, v, depth) -> femur frame (Y, Z, X)
        plate.applyMatrix4(new Matrix4().makeBasis(new Vector3(0, 1, 0), new Vector3(0, 0, 1), new Vector3(1, 0, 0)));

        const b = new ModelBuilder();
        // Plate on -X of the axis, horn side; the tibia is shifted by the same amount so it still hugs the plate.
        b.add(plate, Mat.alu, -0.055, 0, 0);
        // Bent lip along the top edge.
        b.box(0.07, 0.62, 0.02, Mat.alu, -0.065, 0.42, -0.11);
        for (const u of [0.18, 0.74]) {
            b.cyl(0.013, 0.008, Mat.steel, -0.0615, u, 0, 'x', 10);
        }
        b.build(this.femurBody);

        this.femurBody.add(this.tibia);
        this.add(this.femurBody);
    }

    setAngle(angle: number) {
        this.servo.setAngle(angle);
    }

    update(dt: number = 1 / 60) {
        this.femurBody.rotation.x = MathUtils.degToRad(this.servo.smooth(dt) - 90);
    }
}

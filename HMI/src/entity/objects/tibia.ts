import { ExtrudeGeometry, Group, MathUtils, Matrix4, Object3D, Path, Shape, SphereGeometry, Vector2, Vector3 } from 'three'
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";
import { LEG, Mat, ModelBuilder, orient } from "./materials.ts";

// Frame: origin on the tibia axis (along X), +Y runs along the tibia. The tibia servo is bolted to the
// tapered bracket and its horn clamps onto the femur, so the servo swings with the tibia.
export default class Tibia extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = LEG.tibia;
    tibiaBody: Group;
    servo: Servo;
    angle: number = 0;
    // Centre of the rubber foot tip in the tibia frame.
    readonly footLocal = new Vector3(0.18, LEG.tibia, 0);
    socket: Socket;

    constructor(id: number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;
        this.position.set(x, y, z);
        this.rotation.set(MathUtils.degToRad(90), 0, 0);
        this.tibiaBody = new Group();

        const R = 0.2;
        const tipLength = 1.2;
        const tipHalfWidth = 0.03;
        const outline: Vector2[] = [];
        for (let i = 0; i <= 18; i++) {
            const t = Math.PI / 2 + (Math.PI * i) / 18;
            outline.push(new Vector2(R * Math.cos(t), R * Math.sin(t)));
        }
        outline.push(new Vector2(tipLength, -tipHalfWidth), new Vector2(tipLength, tipHalfWidth));
        const shape = new Shape(outline);

        // Window for the servo case.
        const window = new Path();
        window.moveTo(-0.105, -0.103);
        window.lineTo(0.31, -0.103);
        window.lineTo(0.31, 0.103);
        window.lineTo(-0.105, 0.103);
        window.closePath();
        shape.holes.push(window);
        for (const [u, r] of [[0.52, 0.03], [0.7, 0.024]] as const) {
            const hole = new Path();
            hole.absarc(u, 0, r, 0, Math.PI * 2, true);
            shape.holes.push(hole);
        }

        const thickness = 0.025;
        const plate = new ExtrudeGeometry(shape, { depth: thickness, bevelEnabled: false, curveSegments: 16 });
        // Shape (u, v, depth) -> tibia frame (Y, Z, X)
        plate.applyMatrix4(new Matrix4().makeBasis(new Vector3(0, 1, 0), new Vector3(0, 0, 1), new Vector3(1, 0, 0)));

        const footX = this.footLocal.x;
        const b = new ModelBuilder();
        b.add(plate, Mat.alu, 0.1675, 0, 0);
        b.cyl(0.03, 0.14, Mat.rubber, footX, this.height - 0.07, 0, 'y', 16, 0.04);
        b.add(new SphereGeometry(LEG.footRadius, 16, 12), Mat.rubber, footX, this.height, 0);
        b.build(this.tibiaBody);

        // Case on +X, horn against the femur plate, ears resting on the bracket.
        this.servo = new Servo('tibia', id, 0.085, 0, 0, this.socket);
        orient(this.servo, new Vector3(0, 1, 0), new Vector3(-1, 0, 0), new Vector3(0, 0, 1));
        this.tibiaBody.add(this.servo);

        this.add(this.tibiaBody);
    }

    setAngle(angle: number) {
        this.servo.setAngle(angle);
    }

    update(dt: number = 1 / 60) {
        this.tibiaBody.rotation.x = MathUtils.degToRad(this.servo.smooth(dt) - 90);
    }
}

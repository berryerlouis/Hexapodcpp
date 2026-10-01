import Socket from "../../communication/socket.ts";
import Message from "../../communication/message.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterProximityCommands} from "../../communication/clusters/clusterProximity.ts";
import {ClusterSoundCommands} from "../../communication/clusters/clusterSound.ts";
import SoundObject, {SoundSide} from "./sound.ts";
import {
    CanvasTexture,
    CatmullRomCurve3,
    ExtrudeGeometry,
    Group,
    Mesh,
    MeshStandardMaterial,
    Object3D,
    RepeatWrapping,
    Shape,
    TubeGeometry,
    Vector2,
    Vector3,
} from "three";
import ProximityObject, {ProximitySide} from "./proximity.ts";
import { LEG, Mat, ModelBuilder, enableShadows } from "./materials.ts";

interface Proximity {
    left: number;
    front: number[][];
    right: number;
}

interface Sound {
    left: number;
    right: number;
}

interface Sensors {
    proximity: Proximity;
    sound: Sound;
}

// Colour of the 3D printed shell.
const PRINT_COLOR = '#16181c';

function makeLayerLines(): CanvasTexture {
    const canvas = document.createElement('canvas');
    canvas.width = 8;
    canvas.height = 64;
    const ctx = canvas.getContext('2d')!;
    ctx.fillStyle = '#808080';
    ctx.fillRect(0, 0, 8, 64);
    ctx.fillStyle = '#a4a4a4';
    for (let y = 0; y < 64; y += 2) {
        ctx.fillRect(0, y, 8, 1);
    }
    const texture = new CanvasTexture(canvas);
    texture.wrapS = RepeatWrapping;
    texture.wrapT = RepeatWrapping;
    texture.repeat.set(1, 3);
    return texture;
}

// Head frame: forward is -Z, up is +Y. The head is a child of the body (which looks along +X),
// so it is rotated by -90 deg around Y and sits just in front of the plates.
export default class Head extends Object3D {
    static readonly DEPTH = 0.5;
    static readonly HEIGHT = 0.34;
    static readonly FRONT_OFFSET = 1.3;

    sensors: Sensors = {
        proximity: {
            left: 0,
            right: 0,
            front: Array.from({ length: 8 }, () => Array(8).fill(0))
        },
        sound: {
            left: 0,
            right: 0
        }
    }
    socket: Socket;
    sound: SoundObject;
    proximity: ProximityObject;
    model: Group;

    constructor(socket: Socket) {
        super();
        this.socket = socket;
        this.sound = new SoundObject();
        this.proximity = new ProximityObject(this.socket);

        this.position.set(Head.FRONT_OFFSET, LEG.mountY - (LEG.topPlateZ + LEG.bottomPlateZ) / 2, 0);
        this.rotation.y = -Math.PI / 2;
        this.model = this.buildHeadModel();
        this.add(this.model);
        enableShadows(this.model);

        this.proximity.position.set(0, 0, -0.3);
        this.sound.position.set(0, 0.2, 0);

        this.add(this.sound);
        this.add(this.proximity);

        this.addProximityCallbacks();
        this.addSoundCallbacks();
    }

    private buildHeadModel(): Group {
        const model = new Group();
        const D = Head.DEPTH / 2;
        const H = Head.HEIGHT;

        const shellMat = new MeshStandardMaterial({
            color: PRINT_COLOR,
            metalness: 0.05,
            roughness: 0.72,
            bumpMap: makeLayerLines(),
            bumpScale: 0.6,
        });

        // Top-down outline (x, forward): flat front for the ToF, 60 deg faces for the ultrasonic pair.
        const faceHalf = 0.22;
        const faceLength = 0.46;
        const sin = Math.sin(Math.PI / 6);
        const cos = Math.cos(Math.PI / 6);
        const edge = new Vector2(faceLength * sin, -faceLength * cos);
        const outline = [
            new Vector2(-faceHalf, D),
            new Vector2(faceHalf, D),
            new Vector2(faceHalf + edge.x, D + edge.y),
            new Vector2(faceHalf + edge.x, -D),
            new Vector2(-faceHalf - edge.x, -D),
            new Vector2(-faceHalf - edge.x, D + edge.y),
        ];
        const bevel = 0.02;
        const shellGeometry = new ExtrudeGeometry(new Shape(outline), {
            depth: H - 2 * bevel,
            bevelEnabled: true,
            bevelThickness: bevel,
            bevelSize: bevel,
            bevelOffset: -bevel,
            bevelSegments: 3,
            curveSegments: 4,
        });
        // Shape (x, forward) -> world (x, z = -forward), extrusion -> up.
        shellGeometry.rotateX(-Math.PI / 2);
        shellGeometry.translate(0, -(H - 2 * bevel) / 2, 0);
        model.add(new Mesh(shellGeometry, shellMat));

        const b = new ModelBuilder();

        // Printed bracket sliding between the chassis plates, bolted through both.
        b.box(0.44, 0.2, 0.2, Mat.black, 0, 0, D + 0.1);
        for (const sx of [-0.16, 0.16]) {
            b.cyl(0.03, 0.1, Mat.brass, sx, 0, D + 0.25, 'z', 6);
            b.cyl(0.02, 0.56, Mat.steel, sx, 0, D + 0.3, 'y', 10);
        }

        // Front: VL53L5CX 8x8 time-of-flight board in a recessed bezel.
        const zf = -D;
        b.box(0.36, 0.22, 0.02, Mat.black, 0, 0, zf - 0.006);
        b.box(0.3, 0.17, 0.012, Mat.pcbBlack, 0, 0, zf - 0.022);
        b.box(0.11, 0.05, 0.022, Mat.black, 0, 0, zf - 0.039);
        b.box(0.04, 0.032, 0.006, Mat.glass, -0.026, 0, zf - 0.053);
        b.box(0.04, 0.032, 0.006, Mat.glass, 0.026, 0, zf - 0.053);
        b.cyl(0.01, 0.01, Mat.ledRed, 0.12, 0.07, zf - 0.03, 'z', 10);
        for (const sx of [-1, 1]) {
            for (const sy of [-1, 1]) {
                b.cyl(0.01, 0.006, Mat.steel, sx * 0.14, sy * 0.07, zf - 0.03, 'z', 8);
            }
        }

        // Microphones on top, one per side.
        const topY = H / 2;
        for (const sx of [-1, 1]) {
            b.box(0.15, 0.01, 0.1, Mat.pcbPurple, sx * 0.3, topY + 0.004, -0.02);
            b.cyl(0.04, 0.02, Mat.silver, sx * 0.3, topY + 0.019, -0.02, 'y', 20);
            b.cyl(0.022, 0.006, Mat.black, sx * 0.3, topY + 0.03, -0.02, 'y', 16);
        }
        b.cyl(0.01, 0.012, Mat.ledCyan, 0, topY + 0.006, -0.12, 'y', 10);
        b.cyl(0.01, 0.012, Mat.ledGreen, 0.06, topY + 0.006, -0.12, 'y', 10);

        b.build(model);

        model.add(this.createUltrasoundModule(1));
        model.add(this.createUltrasoundModule(-1));
        model.add(this.createCable(D));
        return model;
    }

    // HC-SR04 style module on one of the 60 deg faces. side=1 is the left one (looks toward -X).
    private createUltrasoundModule(side: -1 | 1): Group {
        const module = new Group();
        const D = Head.DEPTH / 2;
        const faceHalf = 0.22;
        const faceLength = 0.46;
        const sin = Math.sin(Math.PI / 6);
        const cos = Math.cos(Math.PI / 6);
        const centerX = faceHalf + (faceLength * sin) / 2;
        const centerForward = D - (faceLength * cos) / 2;
        // Outward normal of the face in (x, forward): (cos 30deg, sin 30deg).
        const nx = cos;
        const nf = sin;
        module.position.set(-side * (centerX + nx * 0.012), 0.01, -(centerForward + nf * 0.012));
        module.rotation.y = side * Math.PI / 3;

        const b = new ModelBuilder();
        b.box(0.45, 0.2, 0.012, Mat.pcbBlue, 0, 0, 0);
        for (const sx of [-1, 1]) {
            const x = sx * 0.13;
            b.cyl(0.08, 0.1, Mat.silver, x, 0, -0.056, 'z', 28);
            b.cyl(0.066, 0.006, Mat.black, x, 0, -0.108, 'z', 24);
            b.cyl(0.03, 0.004, Mat.silver, x, 0, -0.111, 'z', 16);
        }
        b.cyl(0.02, 0.014, Mat.silver, 0, 0.06, 0.012, 'z', 10);
        b.box(0.1, 0.025, 0.025, Mat.black, 0, -0.085, 0.018);
        b.build(module);
        return module;
    }

    // Ribbon of wires running from the head back into the chassis.
    private createCable(D: number): Group {
        const group = new Group();
        const mat = new MeshStandardMaterial({ color: '#15161a', roughness: 0.7 });
        const matRed = new MeshStandardMaterial({ color: '#b82a2e', roughness: 0.7 });
        for (const [i, material] of [[0, mat], [1, matRed], [2, mat]] as const) {
            const x = -0.04 + i * 0.04;
            const curve = new CatmullRomCurve3([
                new Vector3(x, -0.05, D),
                new Vector3(x, -0.12, D + 0.12),
                new Vector3(x, -0.06, D + 0.3),
                new Vector3(x, -0.02, D + 0.5),
            ]);
            group.add(new Mesh(new TubeGeometry(curve, 16, 0.009, 6), material));
        }
        return group;
    }

    addProximityCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.PROXIMITY, ClusterProximityCommands.US_LEFT, (message: Message) => {
            if (message.params) {
                this.sensors.proximity.left = message.getValueUint16(0);
                this.proximity.show(this.sensors.proximity.left, ProximitySide.left);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.PROXIMITY, ClusterProximityCommands.US_RIGHT, (message: Message) => {
            if (message.params) {
                this.sensors.proximity.right = message.getValueUint16(0);
                this.proximity.show(this.sensors.proximity.right, ProximitySide.right);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.PROXIMITY, ClusterProximityCommands.LASER, (message: Message) => {
            // Replies to a manual read only carry the minimum distance, not the 8x8 matrix.
            if (message.params && message.params.length >= 128) {
                for (let i = 0; i < 8; i++) {
                    for (let j = 0; j < 8; j++) {
                        this.sensors.proximity.front[i][j] = message.getValueUint16((i * 8 + j) * 2) / 10;
                    }
                }
                this.proximity.show(this.sensors.proximity.front, ProximitySide.center);
            }
        });
    }

    addSoundCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.SOUND, ClusterSoundCommands.SOUND_STATUS, (message: Message) => {
            if (message.params) {
                if (message.getValueUint8(0) == 0) {
                    this.sensors.sound.left = message.getValueUint24(1);
                    let val = Math.min(Math.max(Math.ceil(this.sensors.sound.right / 2000), 1), 3);
                    this.sound.show(val, SoundSide.left);
                } else {
                    this.sensors.sound.right = message.getValueUint24(1);
                    let val = Math.min(Math.max(Math.ceil(this.sensors.sound.right / 2000), 1), 3);
                    this.sound.show(val, SoundSide.right);
                }
            }
        });
    }

    update() {
        this.proximity.update();
    }
}

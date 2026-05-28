import {
    BoxGeometry,
    CylinderGeometry,
    ExtrudeGeometry,
    Mesh,
    MeshStandardMaterial,
    Object3D,
    Shape,
} from 'three'
import Legs from "./legs.ts";
import Socket from "../../communication/socket.ts";
import { ClusterName } from "../../communication/clusters/clusterType.ts";
import { ClusterServoCommands } from "../../communication/clusters/clusterServo.ts";
import Message from "../../communication/message.ts";

interface BodyStruct {
    width: number;
    widthMiddle: number;
    height: number;
    thickness: number;
    color: string;
}

export default class Body extends Object3D {

    interval: number | null = null;
    params: BodyStruct = {
        width: 1.4,
        widthMiddle: 1.8,
        height: 2,
        thickness: 0.2,
        color: '#515151'
    }
    x: number = 0;
    y: number = 0;
    z: number = 0;
    enable: HTMLElement;
    socket: Socket;
    members: Legs;
    intervalEnable: number = 0;
    intervalDisable: number = 0;
    intervalTime: number = 0;

    constructor(x: number, y: number, z: number, socket: Socket, intervalCommand: number) {
        super();
        this.intervalEnable = intervalCommand;
        this.intervalDisable = 1000;
        this.intervalTime = this.intervalEnable;
        this.x = x;
        this.y = y;
        this.z = z;
        this.socket = socket;

        this.members = new Legs(this.x, this.y, this.z, this.params.width, this.params.widthMiddle, this.params.height, this.socket);
        this.add(this.members);

        this.drawBody();

        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_ALL, (message: Message) => {
            if (message.params && message.params.length === 18) {
                for (let i = 0; i < 6; i++) {
                    for (let j = 0; j < 3; j++) {
                        this.members.legs.leg[i].legData.servos[j].angle = message.getValueUint8(i * 3 + j);
                    }
                }
            }
        });

        this.enable = document.getElementById('enable')!;
        this.enable.addEventListener('click', () => {
            if (this.enable.classList.contains('select')) {
                this.intervalTime = this.intervalEnable;
                if (this.interval !== null) {
                    clearInterval(this.interval);
                }
                this.interval = setInterval(() => {
                    this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_ALL));
                }, this.intervalEnable);
            } else {
                this.intervalTime = this.intervalDisable;
                if (this.interval !== null) {
                    clearInterval(this.interval);
                }
                this.interval = setInterval(() => {
                    this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_ALL));
                }, this.intervalDisable);
            }
        });

        this.socket.addCallbackStarted(() => {
            // if (this.interval !== null) {
            //     clearInterval(this.interval);
            // }
            // this.interval = setInterval(() => {
            //     this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_ALL));
            // }, this.intervalTime);
        });

        this.socket.addCallbackStopped(() => {
            if (this.interval !== null) {
                clearInterval(this.interval);
                this.interval = null;
            }
        });
    }

    drawBody(): void {
        // Capers II inspired chassis: two flat hex plates separated by visible
        // standoffs at each of the six leg mounts, with servo blocks on the
        // lower plate and an electronics / battery stack on the upper deck.
        const plateMat = new MeshStandardMaterial({ color: '#d8dadc', metalness: 0.35, roughness: 0.55 });
        const standoffMat = new MeshStandardMaterial({ color: '#c9a24a', metalness: 0.9,  roughness: 0.25 });
        const boardMat = new MeshStandardMaterial({ color: '#1f5d3a', metalness: 0.15, roughness: 0.75 });
        const batteryMat = new MeshStandardMaterial({ color: '#23272b', metalness: 0.2,  roughness: 0.8  });
        const ledMat = new MeshStandardMaterial({ color: '#ff3030', metalness: 0.1,  roughness: 0.3, emissive: '#ff1010', emissiveIntensity: 0.6 });

        const h = this.params.height;       // body length (X)
        const w = this.params.width;        // body width at front/back corners (Z)
        const wm = this.params.widthMiddle; // body width at middle mounts (Z)
        const t = this.params.thickness;

        // Mount points ordered CCW around the body perimeter so they form a
        // valid simple polygon for the chassis plate footprint.
        const mountPoints: [number, number][] = [
            [ h / 2, -w / 2 ],   // Front Left
            [ h / 2,  w / 2 ],   // Front Right
            [ 0,     wm / 2 ],   // Middle Right
            [-h / 2,  w / 2 ],   // Back Right
            [-h / 2, -w / 2 ],   // Back Left
            [ 0,    -wm / 2 ],   // Middle Left
        ];

        const plateThick = t * 0.45;
        const lowerY = this.y + plateThick / 2;
        const standoffHeight = t * -1.8;
        const upperY = lowerY + plateThick / 2 + standoffHeight + plateThick / 2;

        // Build a polygon footprint that reaches slightly past every leg mount
        // so the chassis plate visibly meets each coxa, including the corners.
        const outwardMargin = 0.22;
        const outline = mountPoints.map<[number, number]>(([mx, mz]) => {
            const len = Math.hypot(mx, mz) || 1;
            return [mx + (mx / len) * outwardMargin, mz + (mz / len) * outwardMargin];
        });

        const makePlateGeom = (pts: [number, number][], thick: number): ExtrudeGeometry => {
            const shape = new Shape();
            shape.moveTo(pts[0][0], pts[0][1]);
            for (let i = 1; i < pts.length; i++) shape.lineTo(pts[i][0], pts[i][1]);
            shape.closePath();
            const geom = new ExtrudeGeometry(shape, { depth: thick, bevelEnabled: false });
            // Shape lives in XY with extrusion along +Z; rotate so the plate
            // lies in the XZ plane with thickness along Y, then centre it.
            geom.rotateX(-Math.PI / 2);
            geom.translate(0, thick / 2, 0);
            return geom;
        };

        // --- Lower chassis plate ---------------------------------------------
        const lowerPlate = new Mesh(makePlateGeom(outline, plateThick), plateMat);
        lowerPlate.position.set(this.x, lowerY, 0);
        this.add(lowerPlate);

        // --- Brass standoffs connecting lower and upper plates ---------------
        const standoffGeom = new CylinderGeometry(0.03, 0.03, standoffHeight, 10);
        for (const [mx, mz] of mountPoints) {
            const standoffR = 0.78;
            const standoff = new Mesh(standoffGeom, standoffMat);
            standoff.position.set(
                this.x + mx * standoffR,
                lowerY + plateThick / 2 + standoffHeight / 2 + plateThick,
                mz * standoffR,
            );
            this.add(standoff);
        }

        // --- Upper deck plate (slightly inset) -------------------------------
        const upperOutline = outline.map<[number, number]>(([px, pz]) => [px * 0.93, pz * 0.93]);
        const upperPlate = new Mesh(makePlateGeom(upperOutline, plateThick), plateMat);
        upperPlate.position.set(this.x, upperY, 0);
        this.add(upperPlate);

        // Controller board (PCB) sitting on the upper deck, slightly toward the back
        const board = new Mesh(
            new BoxGeometry(h * 0.55, plateThick * 0.3, w * 0.5),
            boardMat,
        );
        board.position.set(this.x - h * 0.05, upperY + plateThick / 2 + plateThick * 0.3 + 0.35, 0);
        this.add(board);

        // Status LED on the board
        const led = new Mesh(new CylinderGeometry(0.01, 0.01, 0.04, 10), ledMat);
        led.position.set(this.x - h * 0.05 + h * 0.2, upperY + plateThick / 2 + plateThick * 0.6+0.35, -w * 0.2);
        this.add(led);
    }

    update() {
        this.members?.update();
    }

    setDirection(direction: number) {
        this.members?.setDirection(direction);
    }
}
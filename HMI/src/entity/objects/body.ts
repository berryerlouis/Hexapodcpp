import {
    BoxGeometry,
    CylinderGeometry,
    Mesh,
    MeshStandardMaterial,
    Object3D,
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
        const bodyMat   = new MeshStandardMaterial({ color: '#b8b8b8', metalness: 0.5,  roughness: 0.45 });
        const accentMat = new MeshStandardMaterial({ color: '#d0d0d0', metalness: 0.55, roughness: 0.40 });
        const metalMat  = new MeshStandardMaterial({ color: '#e8e8e8', metalness: 0.80, roughness: 0.20 });
        const mountMat  = new MeshStandardMaterial({ color: '#a8a8a8', metalness: 0.55, roughness: 0.45 });

        const h  = this.params.height;       // 2.0  — front-to-back (X)
        const w  = this.params.width;        // 1.4  — front/back leg Z span
        const wm = this.params.widthMiddle;  // 1.8  — middle leg Z span
        const t  = this.params.thickness;    // 0.2

        // Central circular plate — radius just beyond the middle mounts (z = ±wm/2 = ±0.9)
        const centralGeom = new CylinderGeometry(wm / 2 + 0.05, wm / 2 + 0.05, t * 2, 12);
        const centralPlate = new Mesh(centralGeom, bodyMat);
        centralPlate.position.set(this.x, this.y, 0);
        this.add(centralPlate);

        // Front spar — bridges the central plate to the front mounts (x = h/2, z = ±w/2)
        // X size = h/2 + 0.1  → covers [x - h/4 - (h/2+0.1)/2 , x + h/4 + (h/2+0.1)/2]
        //          centred at h/4 → spans from −0.05 to +1.05  ✓  (front mount at x = 1.0)
        // Z size = w + 0.1    → spans ±(w/2+0.05) = ±0.75       ✓  (front mount at z = ±0.7)
        const sparGeom = new BoxGeometry(h / 2 + 0.1, t * 2, w + 0.1);
        const frontSpar = new Mesh(sparGeom, bodyMat);
        frontSpar.position.set(this.x + 0.25 + h / 4, this.y, 0);
        this.add(frontSpar);

        // Back spar — mirrored
        const backSpar = new Mesh(sparGeom, bodyMat);
        backSpar.position.set(this.x - 0.25 - h / 4, this.y, 0);
        this.add(backSpar);

        // Thin upper cover plate over the central electronics area
        const coverGeom = new CylinderGeometry(wm * 0.45, wm * 0.45, t, 8, 1, false);
        const coverPlate = new Mesh(coverGeom, accentMat);
        coverPlate.rotation.y = Math.PI / 8;
        coverPlate.position.set(this.x, this.y + t * 1.5, 0);
        this.add(coverPlate);

        // Central electronics hub
        const hubGeom = new CylinderGeometry(0.18, 0.28, 0.20, 12, 1, false);
        const hub = new Mesh(hubGeom, accentMat);
        hub.position.set(this.x, this.y + t * 2.0, 0);
        this.add(hub);

        // Leg mount cylinders at the 6 attachment points
        const mountGeom = new CylinderGeometry(0.11, 0.11, t * 4.0, 10);
        const mountPoints: [number, number][] = [
            [  h / 2, -w / 2  ],  // FrontLeft
            [  0,    -wm / 2  ],  // MiddleLeft
            [ -h / 2, -w / 2  ],  // BackLeft
            [  h / 2,  w / 2  ],  // FrontRight
            [  0,     wm / 2  ],  // MiddleRight
            [ -h / 2,  w / 2  ],  // BackRight
        ];
        for (const [mx, mz] of mountPoints) {
            const mount = new Mesh(mountGeom, mountMat);
            mount.position.set(this.x + mx, this.y, mz);
            this.add(mount);
        }

        // Inner standoff pillars
        const standoffGeom = new CylinderGeometry(0.03, 0.03, t * 3.5, 8);
        const standoffPositions: [number, number][] = [
            [  h * 0.30,  w * 0.20 ],
            [  h * 0.30, -w * 0.20 ],
            [ -h * 0.30,  w * 0.20 ],
            [ -h * 0.30, -w * 0.20 ],
        ];
        for (const [px, pz] of standoffPositions) {
            const standoff = new Mesh(standoffGeom, metalMat);
            standoff.position.set(this.x + px, this.y, pz);
            this.add(standoff);
        }
    }

    update() {
        this.members?.update();
    }

    setDirection(direction: number) {
        this.members?.setDirection(direction);
    }
}
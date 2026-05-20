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
        // Capers II inspired chassis: two flat hex plates separated by visible
        // standoffs at each of the six leg mounts, with servo blocks on the
        // lower plate and an electronics / battery stack on the upper deck.
        const plateMat = new MeshStandardMaterial({ color: '#d8dadc', metalness: 0.35, roughness: 0.55 });
        const frameMat = new MeshStandardMaterial({ color: '#1d1f22', metalness: 0.4,  roughness: 0.55 });
        const servoMat = new MeshStandardMaterial({ color: '#111214', metalness: 0.25, roughness: 0.7  });
        const standoffMat = new MeshStandardMaterial({ color: '#c9a24a', metalness: 0.9,  roughness: 0.25 });
        const boardMat = new MeshStandardMaterial({ color: '#1f5d3a', metalness: 0.15, roughness: 0.75 });
        const batteryMat = new MeshStandardMaterial({ color: '#23272b', metalness: 0.2,  roughness: 0.8  });
        const lensMat = new MeshStandardMaterial({ color: '#101216', metalness: 0.7,  roughness: 0.15 });
        const ledMat = new MeshStandardMaterial({ color: '#ff3030', metalness: 0.1,  roughness: 0.3, emissive: '#ff1010', emissiveIntensity: 0.6 });

        const h = this.params.height;       // body length (X)
        const w = this.params.width;        // body width at front/back corners (Z)
        const wm = this.params.widthMiddle; // body width at middle mounts (Z)
        const t = this.params.thickness;

        const mountPoints: [number, number][] = [
            [h / 2, -w / 2],
            [0, -wm / 2],
            [-h / 2, -w / 2],
            [h / 2, w / 2],
            [0, wm / 2],
            [-h / 2, w / 2],
        ];

        const plateThick = t * 0.45;
        const lowerY = this.y + plateThick / 2;
        const standoffHeight = t * 2.8;
        const upperY = lowerY + plateThick / 2 + standoffHeight + plateThick / 2;

        // --- Lower chassis plate (hex) ----------------------------------------
        const plateRadius = wm * 0.6;
        const lowerPlateGeom = new CylinderGeometry(plateRadius, plateRadius, plateThick, 6);
        const lowerPlate = new Mesh(lowerPlateGeom, plateMat);
        lowerPlate.rotation.y = Math.PI / 6;
        lowerPlate.position.set(this.x, lowerY, 0);
        this.add(lowerPlate);

        // Dark structural frame ring on the lower plate (visual edge accent)
        const ringGeom = new CylinderGeometry(plateRadius * 1.01, plateRadius * 1.01, plateThick * 0.35, 6);
        const ring = new Mesh(ringGeom, frameMat);
        ring.rotation.y = Math.PI / 6;
        ring.position.set(this.x, lowerY + plateThick / 2, 0);
        this.add(ring);

        // --- Servo blocks + brass standoffs at each mount ---------------------
        const servoBlockGeom = new BoxGeometry(0.55, t * 1.6, 0.42);
        const standoffGeom = new CylinderGeometry(0.07, 0.07, standoffHeight, 10);

        for (const [mx, mz] of mountPoints) {
            // Servo body (digital servo housing) sitting on top of the lower plate
            const servo = new Mesh(servoBlockGeom, servoMat);
            const yaw = Math.atan2(mz, mx);
            servo.rotation.y = yaw;
            servo.position.set(this.x + mx, lowerY + plateThick / 2 + t * 0.8, mz);
            this.add(servo);

            // Servo horn disc on the outer face of the servo
            const horn = new Mesh(
                new CylinderGeometry(0.13, 0.13, 0.05, 12),
                standoffMat,
            );
            horn.rotation.z = Math.PI / 2;
            horn.rotation.y = yaw;
            const hornOffset = 0.3;
            horn.position.set(
                this.x + mx + Math.cos(yaw) * hornOffset,
                lowerY + plateThick / 2 + t * 0.8,
                mz + Math.sin(yaw) * hornOffset,
            );
            this.add(horn);

            // Brass standoff connecting lower and upper plates (offset inward)
            const standoffR = 0.78;
            const sx = mx * standoffR;
            const sz = mz * standoffR;
            const standoff = new Mesh(standoffGeom, standoffMat);
            standoff.position.set(this.x + sx, lowerY + plateThick / 2 + standoffHeight / 2, sz);
            this.add(standoff);
        }

        // --- Upper deck plate -------------------------------------------------
        const upperPlateRadius = plateRadius * 0.92;
        const upperPlateGeom = new CylinderGeometry(upperPlateRadius, upperPlateRadius, plateThick, 6);
        const upperPlate = new Mesh(upperPlateGeom, plateMat);
        upperPlate.rotation.y = Math.PI / 6;
        upperPlate.position.set(this.x, upperY, 0);
        this.add(upperPlate);

        // Battery pack centered between plates (visible from the side)
        const battery = new Mesh(
            new BoxGeometry(h * 0.7, standoffHeight * 0.7, w * 0.55),
            batteryMat,
        );
        battery.position.set(this.x, lowerY + plateThick / 2 + standoffHeight * 0.45, 0);
        this.add(battery);

        // Controller board (PCB) sitting on the upper deck, slightly toward the back
        const board = new Mesh(
            new BoxGeometry(h * 0.55, plateThick * 0.6, w * 0.65),
            boardMat,
        );
        board.position.set(this.x - h * 0.05, upperY + plateThick / 2 + plateThick * 0.3, 0);
        this.add(board);

        // Status LED on the board
        const led = new Mesh(new CylinderGeometry(0.05, 0.05, 0.04, 10), ledMat);
        led.position.set(this.x - h * 0.05 + h * 0.2, upperY + plateThick / 2 + plateThick * 0.6, w * 0.2);
        this.add(led);

        // --- Forward "head" sensor module -------------------------------------
        const headBase = new Mesh(
            new BoxGeometry(h * 0.18, plateThick * 1.8, w * 0.5),
            frameMat,
        );
        headBase.position.set(this.x + h * 0.36, upperY + plateThick / 2 + plateThick * 0.9, 0);
        this.add(headBase);

        // Camera lens on the head, pointing forward (+X)
        const lens = new Mesh(new CylinderGeometry(0.13, 0.13, 0.08, 16), lensMat);
        lens.rotation.z = Math.PI / 2;
        lens.position.set(this.x + h * 0.46, upperY + plateThick / 2 + plateThick * 0.9, 0);
        this.add(lens);
    }

    update() {
        this.members?.update();
    }

    setDirection(direction: number) {
        this.members?.setDirection(direction);
    }
}
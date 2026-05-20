import { Box3, Object3D } from 'three'
import Socket from "../communication/socket.ts";
import Body from "./objects/body.ts";
import Head from "./objects/head.ts";
import Battery from "./objects/battery.ts";
import Imu from "./objects/imu.ts";
import Button from "./objects/button.ts";
import LoopTime from "./objects/loopTime.ts";
import { ClusterName } from "../communication/clusters/clusterType.ts";
import { ClusterBodyCommands } from "../communication/clusters/clusterBody.ts";
import Message from "../communication/message.ts";
import DirectionArrow from "../engine/directionArrow.ts";

type Gait = 'TRIPOD' | 'WAVE' | 'RIPPLE' | 'DOUBLE_WAVE';

export interface HexapodStruct {
    amplitude: number;
    elevation: number;
    direction: number;
    duration: number;
    rotation: number;
    clockwise: boolean;
    gait: Gait;
    bodyPosition: { x: number, y: number, z: number };
    bodyRotation: { x: number, y: number, z: number };
}

type CallbackMove = () => void;

export default class Hexapod extends Object3D {
    hexapodStruct: HexapodStruct = {
        amplitude: 30,
        elevation: 10,
        direction: 0,
        duration: 1000,
        rotation: 0,
        clockwise: false,
        gait: 'TRIPOD',
        bodyPosition: { x: 0, y: 0, z: 0 },
        bodyRotation: { x: 0, y: 0, z: 0 },
    };
    body: Body;
    socket: Socket;
    head: Head;
    battery: Battery;
    button: Button;
    imu: Imu;
    loopTime: LoopTime;
    directionArrow: DirectionArrow;
    listOfCallbackMove: CallbackMove[];
    isMoving: boolean;
    readonly groundBounds: Box3;
    legSpeedScale: number;
    previousServoAngles: number[];

    constructor(socket: Socket) {
        super();
        this.socket = socket;
        this.button = new Button(this.socket);
        this.head = new Head(this.socket);
        this.directionArrow = new DirectionArrow(this.socket, this.hexapodStruct);
        this.body = new Body(0, 1, 0, this.socket, 50);
        this.imu = new Imu(this.socket, 2000);
        this.battery = new Battery(this.socket, 5000);
        this.loopTime = new LoopTime(this.socket, 1000);
        this.isMoving = false;
        this.groundBounds = new Box3();
        this.legSpeedScale = 0;
        this.previousServoAngles = [];
        this.listOfCallbackMove = [];

        this.body.rotation.y = Math.PI / 2;
        this.add(this.body);
        this.add(this.head);
        this.add(this.directionArrow);

        // Spawn with feet/body resting on the world plane.
        this.alignToGround();


        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ALL_PARAMS, (message: Message) => {
            if (message.params && message.params.length === 9) {
                this.hexapodStruct.amplitude = message.getValueUint8(0);
                this.hexapodStruct.elevation = message.getValueUint8(1);
                this.hexapodStruct.direction = message.getValueUint16(2);
                this.hexapodStruct.rotation = message.getValueUint16(4);
                this.hexapodStruct.clockwise = message.getValueUint8(6) === 1;
                this.hexapodStruct.duration = message.getValueUint16(7);
                this.body.setDirection(this.hexapodStruct.direction);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_DIRECTION, (message: Message) => {
            if (message.params && message.params.length === 2) {
                this.hexapodStruct.direction = message.getValueUint16(0);
                this.body.setDirection(this.hexapodStruct.direction);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_AMPLITUDE, (message: Message) => {
            if (message.params && message.params.length === 1) {
                this.hexapodStruct.amplitude = message.getValueUint8(0);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ELEVATION, (message: Message) => {
            if (message.params && message.params.length === 1) {
                this.hexapodStruct.elevation = message.getValueUint8(0);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_GAIT, (message: Message) => {
            if (message.params && message.params.length === 1) {
                this.hexapodStruct.gait = this.gaitFromId(message.getValueUint8(0));
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS, (message: Message) => {
            if (message.params && message.params.length === 1) {
                this.isMoving = message.getValueUint8(0) !== 2;
            }
        });

        this.socket.addCallbackStarted(() => {
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS, [2, this.hexapodStruct.duration], [0xFF, 0xFFFF]));
        });
    }

    addCallbackMove(callback: CallbackMove) {
        this.listOfCallbackMove.push(callback);
    }

    notifyCallbackMove() {
        this.listOfCallbackMove.forEach((cb) => {
            cb();
        });
    }

    private gaitFromId(gaitId: number): Gait {
        if (gaitId === 1) return 'WAVE';
        if (gaitId === 2) return 'RIPPLE';
        if (gaitId === 3) return 'DOUBLE_WAVE';
        return 'TRIPOD';
    }

    private alignToGround() {
        this.updateMatrixWorld(true);
        this.groundBounds.setFromObject(this.body);
        const minY = this.groundBounds.min.y;
        if (!Number.isFinite(minY)) {
            return;
        }

        const correction = -minY;
        if (Math.abs(correction) > 1e-4) {
            this.position.y += correction;
        }
    }

    private getCurrentServoAngles(): number[] {
        const angles: number[] = [];
        const legs = this.body.members.legs.leg;
        for (let i = 0; i < legs.length; i++) {
            for (let j = 0; j < legs[i].legData.servos.length; j++) {
                angles.push(legs[i].legData.servos[j].angle);
            }
        }
        return angles;
    }

    private updateLegSpeedScale(dt: number): number {
        const angles = this.getCurrentServoAngles();
        if (angles.length === 0) {
            this.legSpeedScale = 0;
            return 0;
        }

        if (this.previousServoAngles.length !== angles.length) {
            this.previousServoAngles = angles;
            this.legSpeedScale = 0;
            return 0;
        }

        const deltas: number[] = [];
        for (let i = 0; i < angles.length; i++) {
            deltas.push(Math.abs(angles[i] - this.previousServoAngles[i]));
        }

        this.previousServoAngles = angles;

        deltas.sort((a, b) => b - a);
        const activeServoCount = Math.max(6, Math.floor(deltas.length / 2));
        let activeDeltaSum = 0;
        for (let i = 0; i < activeServoCount; i++) {
            activeDeltaSum += deltas[i];
        }

        const averageActiveDeltaDeg = activeDeltaSum / activeServoCount;
        const degreesPerSecond = averageActiveDeltaDeg / Math.max(dt, 0.001);

        // Active legs at ~8 deg/s map to 1.0x body speed.
        const targetScale = Math.min(6, Math.max(0, degreesPerSecond / 8));
        const smoothing = Math.min(1, dt * 12);
        this.legSpeedScale += (targetScale - this.legSpeedScale) * smoothing;
        return this.legSpeedScale;
    }

    update(deltaTime: number = 1 / 60) {
        const dt = Math.min(Math.max(deltaTime, 0), 0.1);
        this.body.update();
        this.head.update();

        if (!this.isMoving) {
            return;
        }

        const speedScale = this.updateLegSpeedScale(dt);

        const durationS = Math.max(this.hexapodStruct.duration, 200) / 1000;
        const directionRad = (-this.hexapodStruct.direction * Math.PI) / 180;

        const effectiveSpeedScale = Math.max(1, speedScale);

        const speedBoost = 30;
        const linearSpeed = ((this.hexapodStruct.amplitude / 1000) / durationS) * effectiveSpeedScale * speedBoost;
        const worldAngle = this.rotation.y + directionRad;

        this.position.x += Math.sin(worldAngle) * linearSpeed * dt;
        this.position.z -= Math.cos(worldAngle) * linearSpeed * dt;

        const angularSpeed = (((this.hexapodStruct.rotation * Math.PI) / 180) / durationS) * effectiveSpeedScale * speedBoost;
        const turnSign = this.hexapodStruct.clockwise ? -1 : 1;
        this.rotation.y += angularSpeed * turnSign * dt;
        this.rotation.y = Math.atan2(Math.sin(this.rotation.y), Math.cos(this.rotation.y));

        this.notifyCallbackMove();
    }

    setDirection(direction: number) {
        this.body.setDirection(direction);
        this.directionArrow.setDirection(direction);
    }

    setRotation(rotation: number) {
        this.hexapodStruct.rotation = rotation;
    }

    setRotationClockWize(clockwize: boolean) {
        this.hexapodStruct.clockwise = clockwize;
    }
}
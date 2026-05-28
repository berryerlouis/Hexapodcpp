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
import { Encoding } from "../communication/protocol.ts";
import DirectionArrow from "../engine/directionArrow.ts";
import { Target } from '../connect-target.ts';
import RotationCircle from "./objects/rotationCircle.ts";

type Gait = 'TRIPOD' | 'WAVE' | 'RIPPLE' | 'DOUBLE_WAVE';

export interface HexapodStruct {
    amplitude: number;
    elevation: number;
    direction: number;
    duration: number;
    turningRate: number;
    clockwise: boolean;
    gait: Gait;
    bodyPosition: { x: number, y: number, z: number };
    bodyRotation: { x: number, y: number, z: number };
}

type CallbackMove = () => void;
const ROTATION_RATE_SCALE = 0.25;

export default class Hexapod extends Object3D {
    hexapodStruct: HexapodStruct = {
        amplitude: 30,
        elevation: 10,
        direction: 0,
        duration: 1000,
        turningRate: 0,
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
    rotationCircle: RotationCircle;
    listOfCallbackMove: CallbackMove[];
    isMoving: boolean;
    readonly groundBounds: Box3;
    legSpeedScale: number;
    previousServoAngles: number[];
    simulatedYawOffsetRad: number;
    lastObservedImuYawDeg: number;
    imuHeadingObserved: boolean;
    private targetYawRad: number | null;

    constructor(socket: Socket, target: Target) {
        super();
        this.socket = socket;
        this.button = new Button(this.socket);
        this.head = new Head(this.socket);
        this.directionArrow = new DirectionArrow(this.socket, this.hexapodStruct);
        this.rotationCircle = new RotationCircle(this.socket);
        this.body = new Body(0, 1, 0, this.socket, 100);
        this.imu = new Imu(this.socket, 2000);
        this.battery = new Battery(this.socket, 5000);
        this.loopTime = new LoopTime(this.socket, 1000);
        this.isMoving = false;
        this.groundBounds = new Box3();
        this.legSpeedScale = 0;
        this.previousServoAngles = [];
        this.simulatedYawOffsetRad = 0;
        this.lastObservedImuYawDeg = Number.NaN;
        this.imuHeadingObserved = target === Target.HEXAPOD; // Only trust IMU heading if connected to real hexapod
        this.targetYawRad = null;
        this.listOfCallbackMove = [];

        this.body.rotation.y = Math.PI / 2;
        this.add(this.body);
        this.add(this.head);
        this.add(this.directionArrow);
        this.add(this.rotationCircle);

        // Rotation-circle target callback: compute absolute target yaw.
        this.rotationCircle.onRotationTarget = (offsetDeg: number, clockwise: boolean) => {
            if (offsetDeg === 0) {
                this.targetYawRad = null;
                this.rotationCircle.setRotationTarget(null);
            } else {
                const offsetRad = (offsetDeg * Math.PI) / 180;
                const sign = clockwise ? -1 : 1;
                this.targetYawRad = this.rotation.y + sign * offsetRad;
                this.rotationCircle.setRotationTarget(sign * offsetRad);
            }
        };

        // Spawn with feet/body resting on the world plane.
        this.alignToGround();


        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ALL_PARAMS, (message: Message) => {
            if (message.params && message.params.length === 11) {
                this.hexapodStruct.gait = this.gaitFromId(message.getValueUint8(1));
                this.hexapodStruct.amplitude = message.getValueUint8(2);
                this.hexapodStruct.elevation = message.getValueUint8(3);
                this.hexapodStruct.direction = message.getValueUint16(4);
                this.hexapodStruct.turningRate = message.getValueUint16(6);
                this.hexapodStruct.clockwise = message.getValueUint8(8) === 1;
                this.hexapodStruct.duration = message.getValueUint16(9);
                this.body.setDirection(this.hexapodStruct.direction);
                this.rotationCircle.setDirection(this.hexapodStruct.direction);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_DIRECTION, (message: Message) => {
            if (message.params && message.params.length === 2) {
                this.hexapodStruct.direction = message.getValueUint16(0);
                this.body.setDirection(this.hexapodStruct.direction);
                this.rotationCircle.setDirection(this.hexapodStruct.direction);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ROTATION, (message: Message) => {
            if (message.params && message.params.length === 3) {
                this.hexapodStruct.turningRate = message.getValueUint16(0);
                this.hexapodStruct.clockwise = message.getValueUint8(2) === 1;
                this.rotationCircle.setTurningRate(this.hexapodStruct.turningRate, this.hexapodStruct.clockwise);
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
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_DIRECTION, [0, 0], [0xFF, 0xFF]));
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_ROTATION, [0, 0], [0xFFFF as Encoding, 0xFF as Encoding]));
            this.rotationCircle.enable();
        });

        this.socket.addCallbackStopped(() => {
            this.rotationCircle.disable();
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

    private applyImuRotation() {
        this.hexapodStruct.bodyRotation.x = this.imu.imuData.ypr.pitch;
        this.hexapodStruct.bodyRotation.y = this.imu.imuData.ypr.yaw;
        this.hexapodStruct.bodyRotation.z = this.imu.imuData.ypr.roll;

        const yawRad = (this.hexapodStruct.bodyRotation.y * Math.PI) / 180;
        const pitchRad = (this.hexapodStruct.bodyRotation.x * Math.PI) / 180;
        const rollRad = (this.hexapodStruct.bodyRotation.z * Math.PI) / 180;
        const effectiveYaw = yawRad + (this.imuHeadingObserved ? 0 : this.simulatedYawOffsetRad);

        this.rotation.y = Math.atan2(Math.sin(effectiveYaw), Math.cos(effectiveYaw));
        this.rotationCircle.setHeading((this.rotation.y * 180) / Math.PI);
        this.body.rotation.x = pitchRad;
        this.body.rotation.y = Math.PI / 2;
        this.body.rotation.z = rollRad;
    }

    private updateImuHeadingObservation() {
        const currentYaw = this.imu.imuData.ypr.yaw;
        if (!Number.isFinite(currentYaw)) {
            return;
        }
        this.lastObservedImuYawDeg = currentYaw;
    }

    update(deltaTime: number = 1 / 60) {
        const dt = Math.min(Math.max(deltaTime, 0), 0.1);
        this.updateImuHeadingObservation();
        this.applyImuRotation();
        this.body.update();
        this.head.update();

        if (!this.isMoving) {
            return;
        }

        const speedScale = this.updateLegSpeedScale(dt);

        const durationS = Math.max(this.hexapodStruct.duration, 200) / 1000;
        const directionRad = (this.hexapodStruct.direction * Math.PI) / 180;
        const rotationRateRadPerSec = (((this.hexapodStruct.turningRate * Math.PI) / 180) / durationS) * ROTATION_RATE_SCALE;
        const rotationSign = this.hexapodStruct.clockwise ? -1 : 1;

        if (!this.imuHeadingObserved) {
            this.simulatedYawOffsetRad += rotationRateRadPerSec * rotationSign * dt;
            this.simulatedYawOffsetRad = Math.atan2(
                Math.sin(this.simulatedYawOffsetRad),
                Math.cos(this.simulatedYawOffsetRad),
            );
        }
        this.applyImuRotation();

        // Target-seeking: stop rotation when the target yaw is reached.
        if (this.targetYawRad !== null && this.hexapodStruct.turningRate > 0) {
            const diff = Math.atan2(
                Math.sin(this.targetYawRad - this.rotation.y),
                Math.cos(this.targetYawRad - this.rotation.y),
            );
            // Update the target cursor position each frame as the bot rotates.
            this.rotationCircle.setRotationTarget(diff);
            if (Math.abs(diff) < 0.035) { // ~2 degrees tolerance
                this.targetYawRad = null;
                this.hexapodStruct.turningRate = 0;
                this.hexapodStruct.clockwise = false;
                this.rotationCircle.setTurningRate(0, false);
                this.rotationCircle.setRotationTarget(null);
                this.socket.write(new Message(
                    ClusterName.BODY,
                    ClusterBodyCommands.SET_ROTATION,
                    [0, 0],
                    [0xFFFF as Encoding, 0xFF as Encoding],
                ));
            }
        }

        const effectiveSpeedScale = Math.max(1, speedScale);

        const speedBoost = 3;
        const linearSpeed = ((this.hexapodStruct.amplitude / 1000) / durationS) * effectiveSpeedScale * speedBoost;
        const worldAngle = this.rotation.y + directionRad;

        this.position.x -= Math.sin(worldAngle) * linearSpeed * dt;
        this.position.z -= Math.cos(worldAngle) * linearSpeed * dt;

        this.notifyCallbackMove();
    }

    setDirection(direction: number) {
        this.body.setDirection(direction);
        this.directionArrow.setDirection(direction);
        this.hexapodStruct.direction = direction;
        this.rotationCircle.setDirection(direction);
    }

    setRotation(rotation: number) {
        this.hexapodStruct.turningRate = rotation;
        this.rotationCircle.setTurningRate(rotation, this.hexapodStruct.clockwise);
    }

    setRotationClockwise(clockwise: boolean) {
        this.hexapodStruct.clockwise = clockwise;
        this.rotationCircle.setTurningRate(this.hexapodStruct.turningRate, clockwise);
    }

    setRotationCircleCamera(camera: any) {
        this.rotationCircle.setCamera(camera);
    }

    // ── High-level commands (UI helpers) ─────────────────────────

    /** Set the walk direction (0-360°) and push it to the bot. */
    commitDirection(angleDeg: number) {
        const normalized = ((Math.round(angleDeg) % 360) + 360) % 360;
        this.setDirection(normalized);
        this.socket.write(new Message(
            ClusterName.BODY,
            ClusterBodyCommands.SET_DIRECTION,
            [normalized],
            [0xFFFF as Encoding],
        ));
    }

    /**
     * Set the turning rate from a signed value.
     * Positive = CCW (turn left), negative = CW (turn right).
     */
    commitSignedRotation(signedDeg: number) {
        const clamped = Math.max(-360, Math.min(360, Math.round(signedDeg)));
        const magnitude = Math.abs(clamped);
        const clockwise = clamped < 0;
        this.setRotation(magnitude);
        this.setRotationClockwise(clockwise);
        this.socket.write(new Message(
            ClusterName.BODY,
            ClusterBodyCommands.SET_ROTATION,
            [magnitude, clockwise ? 1 : 0],
            [0xFFFF as Encoding, 0xFF as Encoding],
        ));
    }

    /** Start/stop the gait engine. */
    commitWalkStatus(walking: boolean) {
        this.socket.write(new Message(
            ClusterName.BODY,
            ClusterBodyCommands.SET_WALK_STATUS,
            [walking ? 0 : 2, this.hexapodStruct.duration],
            [0xFF, 0xFFFF as Encoding],
        ));
    }
}
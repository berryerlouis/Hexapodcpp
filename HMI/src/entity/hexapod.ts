import { Box3, Matrix4, Object3D, Quaternion, Vector3 } from 'three'
import Socket from "../communication/socket.ts";
import Body from "./objects/body.ts";
import Head from "./objects/head.ts";
import { enableShadows, LEG } from "./objects/materials.ts";
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

// Feet within this height of the lowest foot (body frame, scene units) count as planted.
const STANCE_BAND = 0.07;

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
    listOfCallbackMove: CallbackMove[];
    isMoving: boolean;
    readonly groundBounds: Box3;
    simulatedYawOffsetRad: number;
    lastObservedImuYawDeg: number;
    imuHeadingObserved: boolean;
    private readonly footBody: Vector3[] = [];
    private readonly footWeight: number[] = [];
    private previousFootBody: Vector3[] | null = null;
    private previousFootWeight: number[] = [];
    private readonly bodyQuaternion = new Quaternion();

    constructor(socket: Socket, target: Target) {
        super();
        this.socket = socket;
        this.button = new Button(this.socket);
        this.head = new Head(this.socket);
        this.directionArrow = new DirectionArrow(this.socket, this.hexapodStruct);
        this.body = new Body(0, 1, 0, this.socket, 100);
        this.imu = new Imu(this.socket, 2000);
        this.battery = new Battery(this.socket, 5000);
        this.loopTime = new LoopTime(this.socket, 1000);
        this.isMoving = false;
        this.groundBounds = new Box3();
        this.simulatedYawOffsetRad = 0;
        this.lastObservedImuYawDeg = Number.NaN;
        this.imuHeadingObserved = target === Target.HEXAPOD; // Only trust IMU heading if connected to real hexapod
        this.listOfCallbackMove = [];

        this.body.rotation.y = Math.PI / 2;
        this.body.add(this.head);
        enableShadows(this.body);
        this.add(this.body);
        this.add(this.directionArrow);

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
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_DIRECTION, (message: Message) => {
            if (message.params && message.params.length === 2) {
                this.hexapodStruct.direction = message.getValueUint16(0);
                this.body.setDirection(this.hexapodStruct.direction);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ROTATION, (message: Message) => {
            if (message.params && message.params.length === 3) {
                this.hexapodStruct.turningRate = message.getValueUint16(0);
                this.hexapodStruct.clockwise = message.getValueUint8(2) === 1;
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

    // Planted feet stay fixed on the floor, so the body travels opposite to their motion in the
    // body frame. Only the servo angles received from the firmware are used (forward kinematics).
    private walkOnFeet(dt: number) {
        const legs = this.body.members.legs.leg;
        this.updateMatrixWorld(true);
        const toBody = new Matrix4().copy(this.body.matrixWorld).invert();
        const world = new Vector3();
        let minBottom = Infinity;
        let lowest = Infinity;
        for (let i = 0; i < legs.length; i++) {
            legs[i].getFootWorldPosition(world);
            minBottom = Math.min(minBottom, world.y - LEG.footRadius);
            if (!this.footBody[i]) {
                this.footBody[i] = new Vector3();
            }
            this.footBody[i].copy(world).applyMatrix4(toBody);
            lowest = Math.min(lowest, this.footBody[i].y);
        }
        for (let i = 0; i < legs.length; i++) {
            this.footWeight[i] = Math.max(0, 1 - (this.footBody[i].y - lowest) / STANCE_BAND);
        }

        if (this.previousFootBody) {
            this.moveBodyFromFeet(this.previousFootBody, this.previousFootWeight);
        }
        this.previousFootBody = this.footBody.map((v) => v.clone());
        this.previousFootWeight = [...this.footWeight];

        // Lowest foot rests on the floor.
        this.position.y -= minBottom * Math.min(1, dt * 10);
    }

    // Weighted 2D rigid fit (x, z) of the planted feet between two frames.
    private moveBodyFromFeet(prev: Vector3[], prevWeight: number[]) {
        let wSum = 0;
        let cax = 0, caz = 0, cbx = 0, cbz = 0;
        let count = 0;
        for (let i = 0; i < prev.length; i++) {
            const w = Math.min(prevWeight[i], this.footWeight[i]);
            if (w <= 0) continue;
            wSum += w;
            count++;
            cax += w * prev[i].x;
            caz += w * prev[i].z;
            cbx += w * this.footBody[i].x;
            cbz += w * this.footBody[i].z;
        }
        if (wSum < 1e-3) {
            return;
        }
        cax /= wSum; caz /= wSum; cbx /= wSum; cbz /= wSum;

        let dot = 0;
        let cross = 0;
        for (let i = 0; i < prev.length; i++) {
            const w = Math.min(prevWeight[i], this.footWeight[i]);
            if (w <= 0) continue;
            const ax = prev[i].x - cax, az = prev[i].z - caz;
            const bx = this.footBody[i].x - cbx, bz = this.footBody[i].z - cbz;
            dot += w * (ax * bx + az * bz);
            cross += w * (ax * bz - az * bx);
        }
        const theta = count >= 2 ? Math.atan2(cross, dot) : 0;
        const c = Math.cos(theta);
        const s = Math.sin(theta);

        // Body motion in the previous body frame: -R^T t, with t = cb - R ca.
        const move = new Vector3(cax - (cbx * c + cbz * s), 0, caz - (-cbx * s + cbz * c));
        this.body.getWorldQuaternion(this.bodyQuaternion);
        move.applyQuaternion(this.bodyQuaternion);
        move.y = 0;
        this.position.add(move);

        if (!this.imuHeadingObserved) {
            this.simulatedYawOffsetRad += theta;
        }
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
        this.body.update(dt);
        this.head.update();
        this.walkOnFeet(dt);
        this.applyImuRotation();

        this.notifyCallbackMove();
    }

    setDirection(direction: number) {
        this.body.setDirection(direction);
        this.directionArrow.setDirection(direction);
        this.hexapodStruct.direction = direction;
    }

    setRotation(rotation: number) {
        this.hexapodStruct.turningRate = rotation;
    }

    setRotationClockwise(clockwise: boolean) {
        this.hexapodStruct.clockwise = clockwise;
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

    /** Start/stop the gait engine; the transition defaults to the step duration. */
    commitWalkStatus(walking: boolean, transitionMs: number = this.hexapodStruct.duration) {
        this.socket.write(new Message(
            ClusterName.BODY,
            ClusterBodyCommands.SET_WALK_STATUS,
            [walking ? 0 : 2, transitionMs],
            [0xFF, 0xFFFF as Encoding],
        ));
    }

    /** Stride amplitude in tenths of a unit (0 = march in place, 30 = full stride). */
    commitAmplitude(amplitude: number) {
        const clamped = Math.max(0, Math.min(30, Math.round(amplitude)));
        this.socket.write(new Message(
            ClusterName.BODY,
            ClusterBodyCommands.SET_AMPLITUDE,
            [clamped],
            [0xFF],
        ));
    }
}
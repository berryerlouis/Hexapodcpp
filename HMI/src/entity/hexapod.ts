import { Object3D } from 'three'
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

    constructor(socket: Socket) {
        super();
        this.socket = socket;
        this.button = new Button(this.socket);
        this.head = new Head(this.socket);
        this.directionArrow = new DirectionArrow(this.socket, this.hexapodStruct);
        this.body = new Body(0, 1, 0, this.socket, 100);
        this.imu = new Imu(this.socket, 1000);
        this.battery = new Battery(this.socket, 1000);
        this.loopTime = new LoopTime(this.socket, 1000);
        this.isMoving = false;
        this.listOfCallbackMove = [];

        this.body.rotation.y = Math.PI / 2;
        this.add(this.body);
        this.add(this.head);
        this.add(this.directionArrow);


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

    update(deltaTime: number = 1 / 60) {
        deltaTime;
        this.body.update();
        this.head.update();

        /*if (this.isMoving) {
            // Calculate actual speed based on amplitude and duration
            // amplitude is in mm, duration is the time for one step cycle in ms
            // Convert to meters per second: (amplitude in mm / 1000) / (duration in ms / 1000)
            const speedMetersPerSecond = (this.hexapodStruct.amplitude / 1000) / (this.hexapodStruct.duration / 1000);

            if (this.hexapodStruct.rotation === 0) {
                // Linear movement: forward/backward based on direction
                // The hexapod's local forward direction
                const directionRadians = (this.hexapodStruct.direction * Math.PI) / 180;
                const hexapodYaw = this.rotation.y;

                // Calculate world space direction
                // Combine the hexapod's rotation with the movement direction
                const worldAngle = hexapodYaw + directionRadians;

                // Calculate displacement based on speed and delta time
                const displacement = speedMetersPerSecond * deltaTime;

                // Update position in world coordinates
                // Note: In Three.js, -Z is forward for default orientation
                this.position.x += Math.sin(worldAngle) * displacement;
                this.position.z += Math.cos(worldAngle) * displacement;
            } else {
                // Rotational movement: turn in place
                // rotation value represents the rotation angle per step
                // Calculate angular velocity based on rotation value and duration
                // rotation is in degrees, duration is in ms
                const rotationDegreesPerSecond = (this.hexapodStruct.rotation / (this.hexapodStruct.duration / 1000));
                const rotationRadiansPerSecond = (rotationDegreesPerSecond * Math.PI) / 180;

                // Apply rotation (clockwise = negative rotation)
                const angularDisplacement = rotationRadiansPerSecond * deltaTime * (this.hexapodStruct.clockwise ? -1 : 1);
                this.rotation.y += angularDisplacement;

                // Normalize rotation to keep it within -PI to PI range
                while (this.rotation.y > Math.PI) this.rotation.y -= 2 * Math.PI;
                while (this.rotation.y < -Math.PI) this.rotation.y += 2 * Math.PI;
            }

            // Notify movement callbacks
            this.notifyCallbackMove();
        }*/
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
import {Object3D} from 'three'
import Socket from "../communication/socket.ts";
import Body from "./objects/body.ts";
import Head from "./objects/head.ts";
import Battery from "./objects/battery.ts";
import Imu from "./objects/imu.ts";
import Button from "./objects/button.ts";
import LoopTime from "./objects/loopTime.ts";
import {ClusterName} from "../communication/clusters/clusterType.ts";
import {ClusterBodyCommands} from "../communication/clusters/clusterBody.ts";
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
        duration: 3000,
        rotation: 0,
        clockwise: false,
        gait: 'TRIPOD',
        bodyPosition: {x: 0, y: 0, z: 0},
        bodyRotation: {x: 0, y: 0, z: 0},
    };
    interval: number;
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
        this.interval = 0;
        this.socket = socket;
        this.button = new Button(this.socket);
        this.head = new Head(this.socket);
        this.directionArrow = new DirectionArrow(this.socket, this.hexapodStruct);
        this.body = new Body(0, 1, 0, this.socket, 100);
        this.imu = new Imu(this.socket, 5000);
        this.battery = new Battery(this.socket, 10000);
        this.loopTime = new LoopTime(this.socket, 10000);
        this.isMoving = false;
        this.listOfCallbackMove = [];

        this.body.rotation.y = Math.PI / 2;
        this.add(this.body);
        this.add(this.head);
        this.add(this.directionArrow);


        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ALL_PARAMS, (message: Message) => {
            if (message.params && message.params.length == 9) {
                this.hexapodStruct.amplitude = message.getValueUint8(0);
                this.hexapodStruct.elevation = message.getValueUint8(1);
                this.hexapodStruct.direction = message.getValueUint16(2);
                this.hexapodStruct.rotation = message.getValueUint16(4);
                this.hexapodStruct.clockwise = message.getValueUint8(6) == 1;
                this.hexapodStruct.duration = message.getValueUint16(7);
                this.body.setDirection(this.hexapodStruct.direction);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_DIRECTION, (message: Message) => {
            if (message.params && message.params.length == 2) {
                this.hexapodStruct.direction = message.getValueUint16(0);
                this.body.setDirection(this.hexapodStruct.direction);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_AMPLITUDE, (message: Message) => {
            if (message.params && message.params.length == 1) {
                this.hexapodStruct.amplitude = message.getValueUint8(0);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ELEVATION, (message: Message) => {
            if (message.params && message.params.length == 1) {
                this.hexapodStruct.elevation = message.getValueUint8(0);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS, (message: Message) => {
            if (message.params && message.params.length == 1) {
                this.isMoving = message.getValueUint8(0) != 2;
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

    update() {
        this.body.update();
        this.head.update();

        if (this.isMoving) {
            if (this.hexapodStruct.rotation == 0) {
                let angle = this.hexapodStruct.direction / 180 * Math.PI;
                this.position.z -= Math.cos(angle) * this.hexapodStruct.duration / 1000000;
                this.position.x -= Math.sin(angle) * this.hexapodStruct.duration / 1000000;
            } else {
                this.rotation.y += (this.hexapodStruct.clockwise ? -1 : 1) * this.hexapodStruct.duration / 1000000;
            }
        }
    }

    setDirection(direction: number) {
        this.body.setDirection(direction);
        this.directionArrow.setDirection(direction);
    }

    setRotation(rotation: number) {
        this.hexapodStruct.rotation = rotation
    }

    setRotationClockWize(clockwize: boolean) {
        this.hexapodStruct.clockwise = clockwize;
    }
}
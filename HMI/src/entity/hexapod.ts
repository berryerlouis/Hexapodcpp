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

type Gait = 'TRIPOD' | 'WAVE' | 'DOUBLE_WAVE';
export interface HexapodStruct {
    amplitude: number;
    elevation: number;
    direction: number;
    duration: number;
    gait: Gait;
    position: { x: number, y: number, z: number };
    rotation: { x: number, y: number, z: number };
}

type CallbackMove = () => void;

export default class Hexapod extends Object3D {
    hexapodStruct:HexapodStruct = {
        amplitude: 30,
        elevation: 10,
        direction: 0,
        duration: 1000,
        gait: 'TRIPOD',
        position: {x:0,y:0,z:0},
        rotation: {x:0,y:0,z:0}
    };
    interval:number;
    body:Body;
    socket:Socket;
    head: Head;
    battery: Battery;
    button:Button;
    imu: Imu;
    loopTime:LoopTime;
    directionArrow:DirectionArrow;
    listOfCallbackMove: CallbackMove[];
    constructor(socket:Socket) {
        super();
        this.interval = 0;
        this.socket = socket;
        this.button = new Button(this.socket);
        this.head = new Head(this.socket);
        this.directionArrow = new DirectionArrow(this.socket, this.hexapodStruct);
        this.body = new Body(0,1,0, this.socket, 100);
        this.imu = new Imu(this.socket,5000);
        this.battery = new Battery(this.socket,10000);
        this.loopTime = new LoopTime(this.socket,10000);
        this.listOfCallbackMove = [];

        this.body.rotation.y = Math.PI/2;
        this.add(this.body);
        this.add(this.head);
        this.add(this.directionArrow);


        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_DIRECTION_AMPLITUDE_ELEVATION_DURATION, (message:Message) => {
            if(message.params && message.params.length == 6) {
                this.hexapodStruct.amplitude = message.getValueUint8(0);
                this.hexapodStruct.elevation = message.getValueUint8(1);
                this.hexapodStruct.direction = message.getValueUint16(2);
                this.hexapodStruct.duration = message.getValueUint16(4);
                this.body.setDirection(this.hexapodStruct.direction);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_DIRECTION, (message:Message) => {
            if(message.params && message.params.length == 2) {
                this.hexapodStruct.direction = message.getValueUint16(0);
                this.body.setDirection(this.hexapodStruct.direction);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_AMPLITUDE, (message:Message) => {
            if(message.params && message.params.length == 1) {
                this.hexapodStruct.amplitude = message.getValueUint8(0);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ELEVATION, (message:Message) => {
            if(message.params && message.params.length == 1) {
                this.hexapodStruct.elevation = message.getValueUint8(0);
            }
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
        /*this.hexapodStruct.rotation.x = degToRad(this.imu.imuData.ypr.pitch);
        this.hexapodStruct.rotation.z = degToRad(this.imu.imuData.ypr.roll);
        this.hexapodStruct.rotation.y = -degToRad(this.imu.imuData.ypr.yaw);*/

        //this.position.set(this.hexapodStruct.position.x,this.hexapodStruct.position.y,this.hexapodStruct.position.z);
        //this.rotation.set(this.hexapodStruct.rotation.x,this.hexapodStruct.rotation.y,this.hexapodStruct.rotation.z);
        this.body.update();
        this.head.update();
    }

    setDirection(direction:number) {
        this.body.setDirection(direction);
        this.directionArrow.setDirection(direction);
    }

    MoveForward(number: number) {
        this.translateZ(number);
        this.notifyCallbackMove();
    }

    MoveBackward(number: number) {
        this.translateZ(number);
        this.notifyCallbackMove();
    }

    MoveLeft(number: number) {
        this.rotateY(number);
        this.notifyCallbackMove();
    }

    MoveRight(number: number) {
        this.rotateY(number);
        this.notifyCallbackMove();
    }
}
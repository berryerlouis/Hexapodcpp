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


export interface HexapodStruct {
    amplitude: number;
    elevation: number;
    direction: number;
    position: { x: number, y: number, z: number };
    rotation: { x: number, y: number, z: number };
}

export default class Hexapod extends Object3D {
    hexapodStruct:HexapodStruct = {
        amplitude: 30,
        elevation: 10,
        direction: 0,
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
    constructor(socket:Socket) {
        super();
        this.interval = 0;
        this.socket = socket;
        this.button = new Button(this.socket);
        this.head = new Head(this.socket);
        this.body = new Body(0,1,0, this.socket, 100);
        this.imu = new Imu(this.socket,5000);
        this.battery = new Battery(this.socket,10000);
        this.loopTime = new LoopTime(this.socket,10000);

        this.body.rotation.y = Math.PI/2;
        this.add(this.body);
        this.add(this.head);


        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_DIRECTION_AMPLITUDE_ELEVATION, (message:Message) => {
            if(message.params && message.params.length == 4) {
                this.hexapodStruct.direction = message.getValueUint16(2);
                this.hexapodStruct.amplitude = message.getValueUint8(0);
                this.hexapodStruct.elevation = message.getValueUint8(1);
                this.body.setDirection(this.hexapodStruct.direction);
            }
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
    }
}
import {Object3D} from 'three'
import Socket from "../communication/socket.ts";
import Body from "./objects/body.ts";
import Head from "./objects/head.ts";
import Battery from "./objects/battery.ts";
import Imu from "./objects/imu.ts";
import {ClusterName} from "../communication/clusters/clusterType.ts";
import Message from "../communication/message.ts";
import {ClusterBodyCommands} from "../communication/clusters/clusterBody.ts";
import Button from "./objects/button.ts";
import {ClusterGeneralCommands} from "../communication/clusters/clusterGeneral.ts";


interface HexapodStruct {
    position: { x: number, y: number, z: number };
    rotation: { x: number, y: number, z: number };
}

export default class Hexapod extends Object3D {
    hexapodStruct:HexapodStruct = {
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
    servicesTimeMax:number;
    servicesTimeMin:number;
    constructor(socket:Socket) {
        super();
        this.interval = 0;
        this.socket = socket;
        this.body = new Body(0,1,0, this.socket, 1000);
        this.battery = new Battery(this.socket,5000);
        this.imu = new Imu(this.socket,1000);
        this.head = new Head(this.socket);
        this.button = new Button(this.socket);
        this.servicesTimeMax = 0;
        this.servicesTimeMin = 0;

        this.body.rotation.y = Math.PI/2;
        this.add(this.body);
        this.add(this.head);

        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.SET_BODY_X_Y_Z, (message:Message) => {
            message;
        });

        this.socket.addSpecificCallbackRead(ClusterName.GENERAL, ClusterGeneralCommands.MAX_EXECUTION_TIME, (message:Message) => {
            if(message.getValueUint8(0) == 4) {
                this.servicesTimeMax = message.getValueUint16(1);
                document.getElementById('max-time')!.innerText = this.servicesTimeMax.toString();
            }
        });

        this.socket.addSpecificCallbackRead(ClusterName.GENERAL, ClusterGeneralCommands.MIN_EXECUTION_TIME, (message:Message) => {
            if(message.getValueUint8(0) == 4) {
                this.servicesTimeMin = message.getValueUint16(1);
                document.getElementById('min-time')!.innerText = this.servicesTimeMin.toString();
            }
        });

        this.socket.addCallbackStopped(()=> {
            clearInterval(this.interval);
        });

        this.socket.addCallbackStarted(()=>{
            this.socket.write(new Message( ClusterName.GENERAL, ClusterGeneralCommands.RESET_TIME));
            this.interval = setInterval(()=>{
                this.socket.write(new Message( ClusterName.GENERAL, ClusterGeneralCommands.MAX_EXECUTION_TIME));
                this.socket.write(new Message( ClusterName.GENERAL, ClusterGeneralCommands.MIN_EXECUTION_TIME));
            }, 10000);
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
}
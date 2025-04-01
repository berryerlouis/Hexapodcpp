import {Object3D} from 'three'
import Socket from "../communication/socket.ts";
import Body from "./objects/body.ts";
import Head from "./objects/head.ts";
import Battery from "./objects/battery.ts";
import Imu from "./objects/imu.ts";
// @ts-ignore
import {degToRad} from "three/src/math/MathUtils";


interface HexapodStruct {
    position: { x: number, y: number, z: number };
    rotation: { x: number, y: number, z: number };
}

export default class Hexapod extends Object3D {
    hexapodStruct:HexapodStruct = {
        position: {x:0,y:0,z:0},
        rotation: {x:0,y:0,z:0}
    };
    body:Body;
    socket:Socket;
    head: Head;
    battery: Battery;
    imu: Imu;
    constructor(socket:Socket) {
        super();
        this.socket = socket;
        this.body = new Body(0,1,0, this.socket);
        this.head = new Head(this.socket);
        this.battery = new Battery(this.socket);
        this.imu = new Imu(this.socket);
        this.body.rotation.y = Math.PI/2;
        this.add(this.body);
        this.add(this.head);
    }

    update() {
        /*this.hexapodStruct.rotation.x = degToRad(this.imu.imuData.ypr.pitch);
        this.hexapodStruct.rotation.z = degToRad(this.imu.imuData.ypr.roll);
        this.hexapodStruct.rotation.y = -degToRad(this.imu.imuData.ypr.yaw);*/
        this.position.set(this.hexapodStruct.position.x,this.hexapodStruct.position.y,this.hexapodStruct.position.z);
        this.rotation.set(this.hexapodStruct.rotation.x,this.hexapodStruct.rotation.y,this.hexapodStruct.rotation.z);
        this.body.update();
        this.head.update();
    }
}
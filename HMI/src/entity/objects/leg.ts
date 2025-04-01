import {MathUtils, Object3D} from 'three'
import Coxys from "./coxys.ts";
import {ServoStruct} from "./servo.ts";
import Socket from "../../communication/socket.ts";

export interface LegStruct {
    name:string;
    servos:ServoStruct[];
}

export default class Leg extends Object3D {
    coxys:Coxys;
    x: number;
    y: number;
    z: number;
    legData:LegStruct;
    socket:Socket;
    constructor(name:string, id:number, x: number, y: number, z: number , leftSide:boolean, socket: Socket) {
        super()
        this.x = x;
        this.y = y;
        this.z = z;
        this.socket = socket;

        this.coxys = new Coxys(id*3,this.x,this.y,this.z, this.socket);
        this.coxys.rotation.set(MathUtils.degToRad(90), 0,leftSide?MathUtils.degToRad(180):0);
        this.add(this.coxys);


        this.legData = {
            name: name,
            servos: [this.coxys.servo.servoData,
                this.coxys.femur.servo.servoData,
                this.coxys.femur.tibia.servo.servoData]
        };
    }

    setCoxysAngle(angle:number) {
        this.coxys.setAngle(angle);
    }
    setFemurAngle(angle:number) {
        this.coxys.femur.setAngle(angle);
    }

    setTibiaAngle(angle:number) {
        this.coxys.femur.tibia.setAngle(angle);
    }

    update() {
        this.coxys.update();
        this.coxys.femur.update();
        this.coxys.femur.tibia.update();
    }


}
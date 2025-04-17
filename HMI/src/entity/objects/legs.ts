import {Object3D} from 'three'
import Leg from "./leg.ts";
import Socket from "../../communication/socket.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import Message from "../../communication/message.ts";
import {ClusterServoCommands} from "../../communication/clusters/clusterServo.ts";

interface LegsStruct {
    status:boolean;
    leg: Leg[];
}
export default class Legs extends Object3D {
    legs: LegsStruct = {
        status: false,
        leg: []
    }
    socket:Socket;
    x: number = 0;
    y: number = 0;
    z: number = 0;

    constructor(x: number, y: number, z: number, bodyWidth: number, bodyWidthMiddle: number, bodyHeight: number, socket: Socket) {
        super();
        this.x = x;
        this.y = y;
        this.z = z;
        this.socket = socket;

        this.createLegs(bodyWidth, bodyWidthMiddle, bodyHeight);
        this.addEnableServoCallbacks();
    }

    createLegs(bodyWidth: number, bodyWidthMiddle: number, bodyHeight: number) {
        this.legs.leg.push(new Leg('FrontLeft',0, this.x + bodyHeight /2, this.y, -bodyWidth / 2 , true, this.socket));
        this.legs.leg.push(new Leg('MiddleLeft',1, this.x, this.y, -bodyWidthMiddle / 2 , true, this.socket));
        this.legs.leg.push(new Leg('BackLeft',2, this.x - bodyHeight /2, this.y, -bodyWidth / 2 , true, this.socket));
        this.legs.leg.push(new Leg('FrontRight',3, this.x - bodyHeight /2, this.y, bodyWidth / 2 , false, this.socket));
        this.legs.leg.push(new Leg('MiddleRight',4, this.x, this.y, bodyWidthMiddle / 2 , false, this.socket));
        this.legs.leg.push(new Leg('BackRight',5, this.x + bodyHeight /2, this.y, bodyWidth / 2 , false, this.socket));
        for (let i = 0; i < 6; i++) {
            this.add(this.legs.leg[i]);
        }
    }
    addEnableServoCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_STATE_PCA,(message:Message) => {
            if(message.params) {
                this.legs.status = message.getValueBool(0);
            }
        });
    }
    update() {
        for (let i = 0; i < 6; i++) {
            this.legs.leg[i].setCoxysAngle(this.legs.leg[i].coxys.servo.getAngle());
            this.legs.leg[i].setFemurAngle(this.legs.leg[i].coxys.femur.servo.getAngle());
            this.legs.leg[i].setTibiaAngle(this.legs.leg[i].coxys.femur.tibia.servo.getAngle());
            this.legs.leg[i].update();
        }
    }
}
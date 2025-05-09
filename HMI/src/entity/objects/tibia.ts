import {
    ConeGeometry,
    MathUtils,
    Mesh,
    MeshPhongMaterial,
    Object3D
} from 'three'
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";

export default class Tibia extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = 1;
    tibiaBody:Mesh;
    servo:Servo;
    angle:number = 0;
    socket:Socket;
    constructor(id:number,x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;
        const geometry = new ConeGeometry( this.width, this.height, 3,3);
        const material = new MeshPhongMaterial({color: '#600000'});
        this.tibiaBody = new Mesh(geometry, material);
        this.position.set(x,y,z);
        this.servo = new Servo('tibia',id,0,0,0,this.socket);
        this.tibiaBody.geometry.translate(0,this.height/2,0 );
        this.rotation.set(MathUtils.degToRad(90),0,0 );

        this.add(this.tibiaBody);
    }

    setAngle(angle:number) {
        this.servo.setAngle(angle);
    }

    update() {
        //this.tibiaBody.rotation.x = MathUtils.degToRad(-this.servo.getAngle() + 90);
        this.tibiaBody.rotation.x = MathUtils.degToRad(this.servo.getAngle() - 90);
    }

}
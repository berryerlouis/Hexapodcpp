import { ConeGeometry, MathUtils, Mesh, MeshPhongMaterial, Object3D} from 'three'
import Femur from "./femur.ts";
import Servo from "./servo.ts";
import Socket from "../../communication/socket.ts";

export default class Coxys extends Object3D {
    readonly width: number = 0.1;
    readonly height: number = 0.3;
    coxysBody:Mesh;
    femur:Femur;
    servo:Servo;
    socket:Socket;

    constructor(id:number, x: number, y: number, z: number, socket: Socket) {
        super()
        this.socket = socket;
        const geometry = new ConeGeometry( this.width, this.height, 3,3);
        const material = new MeshPhongMaterial({color: '#002d60'});
        this.coxysBody = new Mesh(geometry, material);
        this.position.set(x,y,z);
        this.coxysBody.geometry.translate(0,this.height/2,0);

        this.servo = new Servo('coxys',id,0,this.height,0,this.socket);
        this.femur = new Femur(id+1,0,this.height,0,this.socket);
        this.coxysBody.add(this.servo);
        this.coxysBody.add(this.femur);

        this.add(this.coxysBody);
    }

    setAngle(angle:number) {
        this.servo.setAngle(angle);
    }

    update() {
        this.coxysBody.rotation.z = MathUtils.degToRad(this.servo.getAngle() - 90);
    }

}
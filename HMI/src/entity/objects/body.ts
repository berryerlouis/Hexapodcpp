import {
    BoxGeometry,
    BufferGeometry,
    CylinderGeometry,
    DoubleSide,
    Material,
    MathUtils,
    Mesh,
    MeshLambertMaterial,
    Object3D,
    RingGeometry
} from 'three'
import Legs from "./legs.ts";
import Socket from "../../communication/socket.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterServoCommands} from "../../communication/clusters/clusterServo.ts";
import Message from "../../communication/message.ts";

interface BodyStruct {
    width: number;
    widthMiddle: number;
    height: number;
    thickness: number;
    color: string;
}
export default class Body extends Object3D {

    interval: number;
    params:BodyStruct = {
        width: 1.4,
        widthMiddle: 1.8,
        height: 2,
        thickness: 0.2,
        color: '#515151'
    }
    x: number = 0;
    y: number = 0;
    z: number = 0;
    enable:HTMLElement;
    socket:Socket;
    members: Legs;
    intervalEnable: number = 0;
    intervalDisable: number = 0;
    intervalTime: number = 0;
    constructor(x: number, y: number, z: number, socket: Socket, intervalCommand:number) {
        super();
        this.interval = 0;
        this.intervalEnable = intervalCommand;
        this.intervalDisable = 1000;
        this.intervalTime = this.intervalDisable;
        this.x = x;
        this.y = y;
        this.z = z;
        this.socket = socket;

        this.members = new Legs(this.x,this.y,this.z, this.params.width, this.params.widthMiddle, this.params.height, this.socket);
        this.add(this.members);

        this.drawBody();

        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_ALL,(message:Message) => {
            if(message.params && message.params.length == 18) {
                for (let i = 0; i < 6; i++) {
                    for (let j = 0; j < 3; j++) {
                        this.members.legs.leg[i].legData.servos[j].angle = message.getValueUint8(i * 3 + j);
                    }
                }
            }
        });

        this.enable = document.getElementById('enable')!;
        this.enable.addEventListener('click',() => {
            if(!this.enable.classList.contains('select')){
                clearInterval(this.interval);
                this.interval = setInterval(()=>{
                    this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.GET_ALL));
                },this.intervalEnable);
            } else {
                clearInterval(this.interval);
                this.interval = setInterval(()=>{
                    this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.GET_ALL));
                },this.intervalDisable);
            }
        });

        this.socket.addCallbackStarted(()=>{
            clearInterval(this.interval);
            this.interval = setInterval(()=>{
                this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.GET_ALL));
            },this.intervalTime);
        });

        this.socket.addCallbackStopped(()=> {
            clearInterval(this.interval);
        });
    }

    drawBody(): void {
        let geometry: BufferGeometry;
        let material: Material;
        let cylinder: Mesh;

        geometry = new BoxGeometry(this.params.height, this.params.thickness, this.params.width*0.6);
        material = new MeshLambertMaterial({ color: this.params.color });
        const bodyH = new Mesh(geometry, material);
        bodyH.position.x += this.x;
        bodyH.position.y += this.y;
        this.add(bodyH);

        geometry = new BoxGeometry(this.params.widthMiddle*0.6, 0.01, this.params.width*0.3);
        material = new MeshLambertMaterial({ color: this.params.color });
        const bodyW = new Mesh(geometry, material);
        bodyW.position.x += this.x;
        bodyW.position.y += this.y;
        bodyW.rotateY(MathUtils.degToRad(90));
        this.add(bodyW);

        geometry = new CylinderGeometry(this.params.width / 8, this.params.width / 8, this.params.thickness, 32);
        cylinder = new Mesh(geometry, material);
        cylinder.position.z -= this.params.width / 2;
        cylinder.position.x -= this.params.height / 2;
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

        cylinder = new Mesh(geometry, material);
        cylinder.position.z -= this.params.width / 2;
        cylinder.position.x += this.params.height / 2;
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

        cylinder = new Mesh(geometry, material);
        cylinder.position.z += this.params.width / 2;
        cylinder.position.x -= this.params.height / 2;
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

        cylinder = new Mesh(geometry, material);
        cylinder.position.z += this.params.width / 2;
        cylinder.position.x += this.params.height / 2;
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

        cylinder = new Mesh(geometry, material);
        cylinder.position.z -= this.params.widthMiddle / 2;
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

        cylinder = new Mesh(geometry, material);
        cylinder.position.z += this.params.widthMiddle / 2;
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);


        geometry = new RingGeometry(this.params.width / 3, this.params.height / 3, 32, 32, -Math.PI / 2, Math.PI*6/7);
        material = new MeshLambertMaterial({ color: this.params.color, side: DoubleSide });
        cylinder = new Mesh(geometry, material);
        cylinder.position.x -= this.params.width / 2 - 0.15;
        cylinder.position.z -= this.params.widthMiddle / 2;
        cylinder.rotateX(MathUtils.degToRad(90));
        cylinder.rotateZ(MathUtils.degToRad(90));
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

        geometry = new RingGeometry(this.params.width / 3, this.params.height / 3, 32, 32, -Math.PI / 2, Math.PI*6/7);
        material = new MeshLambertMaterial({ color: this.params.color, side: DoubleSide });
        cylinder = new Mesh(geometry, material);
        cylinder.position.x -= this.params.width / 2 - 0.15;
        cylinder.position.z += this.params.widthMiddle / 2;
        cylinder.rotateX(MathUtils.degToRad(90));
        cylinder.rotateZ(MathUtils.degToRad(290));
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

        geometry = new RingGeometry(this.params.width / 3, this.params.height / 3, 32, 32, Math.PI / 2, Math.PI*6/7);
        material = new MeshLambertMaterial({ color: this.params.color, side: DoubleSide });
        cylinder = new Mesh(geometry, material);
        cylinder.position.x += this.params.width / 2 - 0.15;
        cylinder.position.z += this.params.widthMiddle / 2;
        cylinder.rotateX(MathUtils.degToRad(90));
        cylinder.rotateZ(MathUtils.degToRad(90));
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

        geometry = new RingGeometry(this.params.width / 3, this.params.height / 3, 32, 32, -Math.PI / 2, Math.PI*6/7);
        material = new MeshLambertMaterial({ color: this.params.color, side: DoubleSide });
        cylinder = new Mesh(geometry, material);
        cylinder.position.x += this.params.width / 2 - 0.15;
        cylinder.position.z -= this.params.widthMiddle / 2;
        cylinder.rotateX(MathUtils.degToRad(90));
        cylinder.rotateZ(MathUtils.degToRad(110));
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

        geometry = new RingGeometry(0, this.params.height / 4, 32, 32, -Math.PI, Math.PI);
        material = new MeshLambertMaterial({ color: this.params.color, side: DoubleSide });
        cylinder = new Mesh(geometry, material);
        cylinder.position.x += this.params.height - this.params.height/2;
        cylinder.rotateX(MathUtils.degToRad(90));
        cylinder.rotateZ(MathUtils.degToRad(90));
        cylinder.position.x += this.x;
        cylinder.position.y += this.y;
        this.add(cylinder);

    }

    update() {
        this.members?.update();
    }

    setDirection(direction:number) {
        this.members.setDirection(direction);
    }
}
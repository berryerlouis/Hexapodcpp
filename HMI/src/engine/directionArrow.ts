import Socket from "../communication/socket.ts";
import Message from "../communication/message.ts";
import {ClusterName} from "../communication/clusters/clusterType.ts";
import {ClusterServoCommands} from "../communication/clusters/clusterServo.ts";
import {ClusterBodyCommands} from "../communication/clusters/clusterBody.ts";
import {lerp} from "../tool/lerp.ts";

interface LegMovement {
    x: number;
    y: number;
    z: number;
}


export default class DirectionArrow {

    socket:Socket;
    up:HTMLElement;
    right:HTMLElement;
    left:HTMLElement;
    down:HTMLElement;
    stop:HTMLElement;
    enable:HTMLElement;
    legArrayMvt: LegMovement[] = [];
    legArrayMvtIndex: number = 0;
    lerpTime: number = 0;
    interval: number = 0;
    constructor(socket:Socket) {
        this.socket = socket;
        this.up = document.getElementById('up')!;
        this.right = document.getElementById('right')!;
        this.left = document.getElementById('left')!;
        this.down = document.getElementById('down')!;
        this.stop = document.getElementById('stop')!;
        this.enable = document.getElementById('enable')!;

        this.up.addEventListener('click',() => {
            this.up.classList.toggle('select');
            this.right.classList.remove('select');
            this.left.classList.remove('select');
            this.down.classList.remove('select');
            this.stop.classList.remove('select');
            this.socket.write( new Message( ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS,[0]));
        });
        this.right.addEventListener('click',() => {
            this.up.classList.remove('select');
            this.right.classList.toggle('select');
            this.left.classList.remove('select');
            this.down.classList.remove('select');
            this.stop.classList.remove('select');
            this.socket.write( new Message( ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS,[0]));
        });
        this.left.addEventListener('click',() => {
            this.up.classList.remove('select');
            this.right.classList.remove('select');
            this.left.classList.toggle('select');
            this.down.classList.remove('select');
            this.stop.classList.remove('select');
            this.socket.write( new Message( ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS,[0]));
        });
        this.down.addEventListener('click',() => {
            this.up.classList.remove('select');
            this.right.classList.remove('select');
            this.left.classList.remove('select');
            this.down.classList.toggle('select');
            this.stop.classList.remove('select');
            this.socket.write( new Message( ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS,[0]));
        });
        this.stop.addEventListener('click',() => {
            this.up.classList.remove('select');
            this.right.classList.remove('select');
            this.left.classList.remove('select');
            this.down.classList.remove('select');
            this.stop.classList.toggle('select');
            this.socket.write( new Message( ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS,[2]));
        });
        this.enable.addEventListener('click',() => {
            this.enable.classList.toggle('select');
            if(this.enable.classList.contains('select')){
                this.enableServos();
            } else {
                this.disableServos();
            }
        });

        this.legArrayMvtIndex = 0;
        this.lerpTime = 0;
        this.interval = 0;
        this.legArrayMvt.push({x:0,y:-30,z:-10});
        this.legArrayMvt.push({x:0,y:-30,z:0});
        this.legArrayMvt.push({x:0,y:30,z:0});


        this.socket.addCallbackStarted(()=> {
            /*this.interval = setInterval( (this.legArrayMvtIndex+1) % this.legArrayMvt() => {
                let next = .length;
                let next1 = (this.legArrayMvtIndex+2) % this.legArrayMvt.length;
                this.moveLeg(
                    0,
                    lerp(this.legArrayMvt[this.legArrayMvtIndex].x,this.legArrayMvt[next].x,this.lerpTime),
                    lerp(this.legArrayMvt[this.legArrayMvtIndex].y,this.legArrayMvt[next].y,this.lerpTime),
                    lerp(this.legArrayMvt[this.legArrayMvtIndex].z,this.legArrayMvt[next].z,this.lerpTime)
                );
                this.moveLeg(
                    2,
                    lerp(this.legArrayMvt[this.legArrayMvtIndex].x,this.legArrayMvt[next].x,this.lerpTime),
                    lerp(this.legArrayMvt[this.legArrayMvtIndex].y,this.legArrayMvt[next].y,this.lerpTime),
                    lerp(this.legArrayMvt[this.legArrayMvtIndex].z,this.legArrayMvt[next].z,this.lerpTime)
                );
                this.moveLeg(
                    4,
                    lerp(this.legArrayMvt[this.legArrayMvtIndex].x,this.legArrayMvt[next].x,this.lerpTime),
                    lerp(this.legArrayMvt[this.legArrayMvtIndex].y,this.legArrayMvt[next].y,this.lerpTime),
                    lerp(this.legArrayMvt[this.legArrayMvtIndex].z,this.legArrayMvt[next].z,this.lerpTime)
                );
                this.moveLeg(
                    1,
                    lerp(this.legArrayMvt[next].x,this.legArrayMvt[next1].x,this.lerpTime),
                    lerp(this.legArrayMvt[next].y,this.legArrayMvt[next1].y,this.lerpTime),
                    lerp(this.legArrayMvt[next].z,this.legArrayMvt[next1].z,this.lerpTime)
                );
                this.moveLeg(
                    3,
                    lerp(this.legArrayMvt[next].x,this.legArrayMvt[next1].x,this.lerpTime),
                    lerp(this.legArrayMvt[next].y,this.legArrayMvt[next1].y,this.lerpTime),
                    lerp(this.legArrayMvt[next].z,this.legArrayMvt[next1].z,this.lerpTime)
                );
                this.moveLeg(
                    5,
                    lerp(this.legArrayMvt[next].x,this.legArrayMvt[next1].x,this.lerpTime),
                    lerp(this.legArrayMvt[next].y,this.legArrayMvt[next1].y,this.lerpTime),
                    lerp(this.legArrayMvt[next].z,this.legArrayMvt[next1].z,this.lerpTime)
                );
                this.lerpTime += 0.1;
                if(this.lerpTime >= 1) {
                    this.lerpTime = 0;
                    this.legArrayMvtIndex++;
                }
                if(this.legArrayMvtIndex == this.legArrayMvt.length) { this.legArrayMvtIndex = 0;}
            },100);*/
        });
        this.socket.addCallbackStopped(()=> {
            clearInterval(this.interval);
        });
    }

    moveLeg(indexLeg: number, x: number, y: number, z: number) {
        this.socket.write( new Message( ClusterName.BODY, ClusterBodyCommands.SET_LEG_X_Y_Z,
            [
                indexLeg,
                Math.floor(x),
                Math.floor(y),
                Math.floor(z),
                100
            ],
            [
                0xFF,
                0xFFFF,
                0xFFFF,
                0xFFFF,
                0xFFFF,
            ]));
    }

    enableServos() {
        for (let i = 0; i < 18; i++) {
            this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE, [i,1]));
        }
        this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, [1]));
        this.enable.children[0].classList.remove('bi-toggle2-off');
        this.enable.children[0].classList.add('bi-toggle2-on');
        this.enable.children[0].classList.add('select');
    }
    disableServos() {
        for (let i = 0; i < 18; i++) {
            this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE, [i,0]));
        }
        this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, [0]));
        this.enable.children[0].classList.remove('bi-toggle2-on');
        this.enable.children[0].classList.add('bi-toggle2-off');
        this.enable.children[0].classList.remove('select');
    }
}
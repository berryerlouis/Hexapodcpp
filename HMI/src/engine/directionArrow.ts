import Socket from "../communication/socket.ts";
import Message from "../communication/message.ts";
import {ClusterName} from "../communication/clusters/clusterType.ts";
import {ClusterServoCommands} from "../communication/clusters/clusterServo.ts";

export default class DirectionArrow {
    socket:Socket;
    up:HTMLElement;
    right:HTMLElement;
    left:HTMLElement;
    down:HTMLElement;
    stop:HTMLElement;
    enable:HTMLElement;
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
        });
        this.right.addEventListener('click',() => {
            this.up.classList.remove('select');
            this.right.classList.toggle('select');
            this.left.classList.remove('select');
            this.down.classList.remove('select');
            this.stop.classList.remove('select');
        });
        this.left.addEventListener('click',() => {
            this.up.classList.remove('select');
            this.right.classList.remove('select');
            this.left.classList.toggle('select');
            this.down.classList.remove('select');
            this.stop.classList.remove('select');
        });
        this.down.addEventListener('click',() => {
            this.up.classList.remove('select');
            this.right.classList.remove('select');
            this.left.classList.remove('select');
            this.down.classList.toggle('select');
            this.stop.classList.remove('select');
        });
        this.stop.addEventListener('click',() => {
            this.up.classList.remove('select');
            this.right.classList.remove('select');
            this.left.classList.remove('select');
            this.down.classList.remove('select');
            this.stop.classList.toggle('select');
        });
        this.enable.addEventListener('click',() => {
            this.enable.classList.toggle('select');
            if(this.enable.classList.contains('select')){
                this.enableServos();
            } else {
                this.disableServos();
            }
        });
    }

    enableServos() {
        for (let i = 0; i < 18; i++) {
            this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE, 2, [i,1]));
        }
        this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, 1, [1]));
        this.enable.children[0].classList.remove('bi-toggle2-off');
        this.enable.children[0].classList.add('bi-toggle2-on');
        this.enable.children[0].classList.add('select');
    }
    disableServos() {
        for (let i = 0; i < 18; i++) {
            this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE, 2, [i,0]));
        }
        this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, 1, [0]));
        this.enable.children[0].classList.remove('bi-toggle2-on');
        this.enable.children[0].classList.add('bi-toggle2-off');
        this.enable.children[0].classList.remove('select');
    }
}
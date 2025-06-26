import Socket from "../communication/socket.ts";
import Message from "../communication/message.ts";
import {ClusterName} from "../communication/clusters/clusterType.ts";
import {ClusterServoCommands} from "../communication/clusters/clusterServo.ts";
import {ClusterBodyCommands} from "../communication/clusters/clusterBody.ts";
import {CircleGeometry, DoubleSide, MathUtils, Mesh, MeshBasicMaterial, Object3D} from "three";
import {HexapodStruct} from "../entity/hexapod.ts";

export default class DirectionArrow extends Object3D {
    socket: Socket;
    right: HTMLElement;
    forward: HTMLElement;
    left: HTMLElement;
    backward: HTMLElement;
    stop: HTMLElement;
    enable: HTMLElement;
    circle: Mesh;
    hexapodStruct: HexapodStruct;

    constructor(socket: Socket, hexapodStruct: HexapodStruct) {
        super();
        this.socket = socket;
        this.hexapodStruct = hexapodStruct;
        this.forward = document.getElementById('forward')!;
        this.right = document.getElementById('right')!;
        this.left = document.getElementById('left')!;
        this.backward = document.getElementById('backward')!;
        this.stop = document.getElementById('stop')!;
        this.enable = document.getElementById('enable')!;

        this.initializeButtons();

        const geometry = new CircleGeometry(0.3, 32, -Math.PI / 2 - Math.PI / 16 / 2, Math.PI / 16);
        geometry.rotateX(Math.PI / 2);
        geometry.rotateY(Math.PI);
        geometry.translate(0, 1.11, -0.5);
        const material = new MeshBasicMaterial({color: 0x4a6a8f});
        material.side = DoubleSide;
        this.circle = new Mesh(geometry, material);
        //this.circle.visible = false;
        this.add(this.circle);

        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ALL_PARAMS, (message: Message) => {
            if (message.params && message.params.length == 9) {
                this.setDirection(message.getValueUint16(2));
            }
        });

    }

    initializeButtons() {
        this.forward.addEventListener('click', () => {
            this.forward.classList.toggle('select');
            this.right.classList.remove('select');
            this.left.classList.remove('select');
            this.backward.classList.remove('select');
            this.stop.classList.remove('select');
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS, [0, this.hexapodStruct.duration], [0xFF, 0xFFFF]));
        });
        this.right.addEventListener('click', () => {
            this.forward.classList.remove('select');
            this.right.classList.toggle('select');
            this.left.classList.remove('select');
            this.backward.classList.remove('select');
            this.stop.classList.remove('select');
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS, [0, this.hexapodStruct.duration], [0xFF, 0xFFFF]));
        });
        this.left.addEventListener('click', () => {
            this.forward.classList.remove('select');
            this.right.classList.remove('select');
            this.left.classList.toggle('select');
            this.backward.classList.remove('select');
            this.stop.classList.remove('select');
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS, [0, this.hexapodStruct.duration], [0xFF, 0xFFFF]));
        });
        this.backward.addEventListener('click', () => {
            this.forward.classList.remove('select');
            this.right.classList.remove('select');
            this.left.classList.remove('select');
            this.backward.classList.toggle('select');
            this.stop.classList.remove('select');
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS, [0, this.hexapodStruct.duration], [0xFF, 0xFFFF]));
        });
        this.stop.addEventListener('click', () => {
            this.forward.classList.remove('select');
            this.right.classList.remove('select');
            this.left.classList.remove('select');
            this.backward.classList.remove('select');
            this.stop.classList.toggle('select');
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_WALK_STATUS, [2, this.hexapodStruct.duration], [0xFF, 0xFFFF]));
        });
        this.enable.addEventListener('click', () => {
            this.enable.classList.toggle('select');
            if (this.enable.classList.contains('select')) {
                this.enableServos();
            } else {
                this.disableServos();
            }
        });
    }

    setDirection(direction: number) {
        this.circle.rotation.y = (MathUtils.degToRad(direction));
    }

    enableServos() {
        for (let i = 0; i < 18; i++) {
            this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.SET_STATE, [i, 1]));
        }
        this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, [1]));
        this.enable.children[0].classList.remove('bi-toggle2-off');
        this.enable.children[0].classList.add('bi-toggle2-on');
        this.enable.children[0].classList.add('select');
        this.circle.visible = true;
    }

    disableServos() {
        for (let i = 0; i < 18; i++) {
            this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.SET_STATE, [i, 0]));
        }
        this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, [0]));
        this.enable.children[0].classList.remove('bi-toggle2-on');
        this.enable.children[0].classList.add('bi-toggle2-off');
        this.enable.children[0].classList.remove('select');
        this.circle.visible = false;
    }
}
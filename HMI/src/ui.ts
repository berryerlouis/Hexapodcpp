import Graphic from './engine/graphic.ts'
import Camera from './engine/camera.ts'
import Light from './engine/light.ts'
import Control from "./engine/control.ts";
import Hexapod from "./entity/hexapod.ts";
import World from "./entity/world.ts";
import Display from "./engine/scene.ts";
import Panel from "./engine/panel.ts";
import Socket from "./communication/socket.ts";
import Keyboard from "./engine/keyboard.ts";
import { ClusterName } from "./communication/clusters/clusterType.ts";
import { ClusterGeneralCommands } from "./communication/clusters/clusterGeneral.ts";
import Message from "./communication/message.ts";
import { ClusterBodyCommands } from "./communication/clusters/clusterBody.ts";
import { ClusterServoCommands } from "./communication/clusters/clusterServo.ts";
import { ClusterBatteryCommands } from './communication/clusters/clusterBattery.ts';


export default class Ui {
    scene: Display;
    world: World;
    light: Light;
    camera: Camera;
    graphic: Graphic;
    control: Control;
    socket: Socket;
    hexapod: Hexapod;
    panel: Panel;
    keyboard: Keyboard;
    version: HTMLElement;

    constructor(socket: Socket) {
        this.socket = socket;
        this.scene = new Display();
        this.light = new Light()
        this.hexapod = new Hexapod(socket);
        this.camera = new Camera(this.hexapod);
        this.graphic = new Graphic(this.scene, this.camera);
        this.control = new Control(this.camera, this.graphic);
        this.panel = new Panel(document.getElementById('panel')!, this.hexapod, this.socket);
        this.world = new World(20, 20, this.hexapod);
        this.version = document.getElementById('version')!;


        this.keyboard = new Keyboard(this.hexapod);
        this.scene.add(this.world);
        this.scene.add(this.light);
        this.scene.add(this.hexapod);
        this.graphic.onUpdate(dt => {
            this.update(dt);
        });
        this.socket.addCallbackStarted(() => {
            this.initCom();
        });


        this.socket.addSpecificCallbackRead(ClusterName.GENERAL, ClusterGeneralCommands.VERSION, (message: Message) => {
            this.version.innerText = "V" + message.getValueUint8(0).toString() + "." + message.getValueUint8(1).toString();
        });
    }

    initCom() {

        this.socket.write(new Message(ClusterName.BATTERY, ClusterBatteryCommands.STATUS));
        this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.SET_STATE, [0, 1]));
        this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.GET_ALL_PARAMS));
        this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_STATE_PCA));

        for (let i = 0; i < 18; i++) {
            this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_ANGLE, [i]));
            this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_MIN, [i]));
            this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_MAX, [i]));
            this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_STATE, [i]));
        }
        this.socket.write(new Message(ClusterName.GENERAL, ClusterGeneralCommands.VERSION));
    }

    update(dt: number) {
        this.control.update(dt);
        this.world.update();
        this.hexapod.update(dt);
        this.panel.update();
        this.camera.update();
        this.light.update(this.hexapod);
    }
}



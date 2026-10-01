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
import { Target } from './connect-target.ts';
import RangeMap from "./entity/objects/rangeMap.ts";
import RangeSimulator from "./entity/objects/rangeSimulator.ts";
import SensorHud from "./engine/sensorHud.ts";
import DriveController from "./engine/driveController.ts";
import VirtualStick from "./engine/virtualStick.ts";
import GamepadInput from "./engine/gamepadInput.ts";
import PointAndGo from "./engine/pointAndGo.ts";


export default class Ui {
    private static readonly SETTINGS_DOCK_STORAGE_KEY = 'hexapod.settingsDockCollapsed';

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
    drive: DriveController;
    moveStick: VirtualStick;
    turnStick: VirtualStick;
    gamepad: GamepadInput;
    pointAndGo: PointAndGo;
    rangeMap: RangeMap;
    rangeSimulator: RangeSimulator;
    sensorHud: SensorHud;
    version: HTMLElement;
    settingsDock: HTMLElement;
    settingsToggle: HTMLButtonElement;
    private lastHexapodX = 0;
    private lastHexapodZ = 0;

    constructor(socket: Socket, target: Target) {
        this.socket = socket;
        this.scene = new Display();
        this.light = new Light()
        this.hexapod = new Hexapod(socket, target);
        this.camera = new Camera(this.hexapod);
        this.graphic = new Graphic(this.scene, this.camera);
        this.control = new Control(this.camera, this.graphic);
        this.panel = new Panel(document.getElementById('panel')!, this.hexapod, this.socket);
        this.world = new World(20, 20, this.hexapod);
        this.version = document.getElementById('version')!;
        this.settingsDock = document.getElementById('settings-dock')!;
        this.settingsToggle = document.getElementById('toggle-settings') as HTMLButtonElement;

        this.drive = new DriveController(this.hexapod);
        this.moveStick = new VirtualStick(
            document.getElementById('stick-move')!,
            'xy',
            (x, y) => this.drive.setMove('stick', x, y),
        );
        this.turnStick = new VirtualStick(
            document.getElementById('stick-turn')!,
            'x',
            (x) => this.drive.setTurn('stick', x),
        );
        this.pointAndGo = new PointAndGo(this.hexapod, this.drive, this.camera, this.graphic.domElement, this.socket);
        this.gamepad = new GamepadInput(this.drive, () => {
            this.hexapod.commitWalkStatus(false);
            this.pointAndGo.cancel(true);
        });
        document.getElementById('stop')!.addEventListener('click', () => this.pointAndGo.cancel(true));

        // Keys hand the bot over to the keyboard, so a running trip must not fight them.
        this.keyboard = new Keyboard(this.hexapod, () => this.pointAndGo.cancel(false));

        this.rangeMap = new RangeMap();
        this.hexapod.head.proximity.setMap(this.rangeMap);
        this.rangeSimulator = new RangeSimulator(this.hexapod.head.proximity);
        // The X64 target has no range sensors, so it starts on the virtual room.
        this.sensorHud = new SensorHud(
            document.getElementById('sensor-hud')!,
            this.hexapod,
            this.rangeMap,
            this.rangeSimulator,
            this.socket,
            target === Target.X64,
        );

        const savedDockCollapsed = localStorage.getItem(Ui.SETTINGS_DOCK_STORAGE_KEY);
        if (savedDockCollapsed === '1') {
            this.settingsDock.classList.add('is-collapsed');
        } else if (savedDockCollapsed === '0') {
            this.settingsDock.classList.remove('is-collapsed');
        }

        this.scene.add(this.world);
        this.scene.add(this.light);
        this.scene.add(this.hexapod);
        this.scene.add(this.pointAndGo);
        this.scene.add(this.rangeMap);
        this.graphic.onUpdate(dt => {
            this.update(dt);
        });
        setInterval(() => this.panel.update(), 500);

        this.settingsToggle.addEventListener('click', () => {
            this.settingsDock.classList.toggle('is-collapsed');
            localStorage.setItem(
                Ui.SETTINGS_DOCK_STORAGE_KEY,
                this.settingsDock.classList.contains('is-collapsed') ? '1' : '0',
            );
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
            this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.GET_SERVO_ALL, [i]));
        }
        this.socket.write(new Message(ClusterName.GENERAL, ClusterGeneralCommands.VERSION));
    }

    private followHexapod() {
        const p = this.hexapod.position;
        const dx = p.x - this.lastHexapodX;
        const dz = p.z - this.lastHexapodZ;
        this.lastHexapodX = p.x;
        this.lastHexapodZ = p.z;
        this.camera.position.x += dx;
        this.camera.position.z += dz;
        this.control.target.x += dx;
        this.control.target.z += dz;
    }

    private updateRangeSensors() {
        const now = performance.now();
        this.rangeSimulator.update(now);
        this.rangeMap.addTrailPoint(this.hexapod.position.x, this.hexapod.position.z);
        this.rangeMap.update(now);
        this.sensorHud.update(now);
    }

    update(dt: number) {
        this.followHexapod();
        this.control.update(dt);
        this.world.update();
        this.hexapod.update(dt);
        const now = performance.now();
        this.gamepad.update();
        this.pointAndGo.update(now);
        this.drive.update(now);
        this.updateRangeSensors();
        this.camera.update();
        this.light.update(this.hexapod);
    }
}



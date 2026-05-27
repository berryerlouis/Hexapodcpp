import Socket from "../../communication/socket.ts";
import Message from "../../communication/message.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterProximityCommands} from "../../communication/clusters/clusterProximity.ts";
import {ClusterSoundCommands} from "../../communication/clusters/clusterSound.ts";
import SoundObject, {SoundSide} from "./sound.ts";
import {
    BoxGeometry,
    CylinderGeometry,
    Group,
    Mesh,
    MeshStandardMaterial,
    Object3D,
    SphereGeometry
} from "three";
import ProximityObject, {ProximitySide} from "./proximity.ts";

interface Proximity {
    left: number;
    front: number[][];
    right: number;
}

interface Sound {
    left: number;
    right: number;
}

interface Sensors {
    proximity: Proximity;
    sound: Sound;
}

export default class Head extends Object3D {
    sensors: Sensors = {
        proximity: {
            left: 0,
            right: 0,
            front: Array.from({ length: 8 }, () => Array(8).fill(0))
        },
        sound: {
            left: 0,
            right: 0
        }
    }
    socket: Socket;
    sound: SoundObject;
    proximity: ProximityObject;

    constructor(socket: Socket) {
        super();
        this.socket = socket;
        this.sound = new SoundObject();
        this.proximity = new ProximityObject(this.socket);

        this.position.set(0, 1, -1.5);
        this.buildHeadModel();

        this.proximity.position.set(0, 0.22, -0.48);
        this.sound.position.set(0, 0.38, 0.02);

        this.add(this.sound);
        this.add(this.proximity);

        this.addProximityCallbacks();
        this.addSoundCallbacks();
    }

    private buildHeadModel() {
        const shellMat = new MeshStandardMaterial({ color: '#c4c7cb', metalness: 0.55, roughness: 0.36 });
        const darkMat = new MeshStandardMaterial({ color: '#4a4d54', metalness: 0.45, roughness: 0.5 });
        const sensorMat = new MeshStandardMaterial({ color: '#9fd6ff', metalness: 0.2, roughness: 0.15, emissive: '#0a1526' });
        const laserMat = new MeshStandardMaterial({ color: '#ff9f9f', metalness: 0.2, roughness: 0.15, emissive: '#0a1526' });
        const micMat = new MeshStandardMaterial({ color: '#181a1f', metalness: 0.35, roughness: 0.45 });

        const neck = new Mesh(new CylinderGeometry(0.22, 0.18, 0.18, 18), darkMat);
        neck.position.set(0, -0.05, 0.05);
        this.add(neck);

        const headShell = new Mesh(new CylinderGeometry(0.56, 0.64, 0.34, 3), shellMat);
        headShell.position.set(0, 0.12, -0.02);
        this.add(headShell);

        const topPlate = new Mesh(new CylinderGeometry(0.42, 0.5, 0.08, 3), darkMat);
        topPlate.position.set(0, 0.33, -0.02);
        this.add(topPlate);

        const laserHousing = new Mesh(new BoxGeometry(0.38, 0.2, 0.22), darkMat);
        laserHousing.position.set(0, 0.12, -0.30);
        this.add(laserHousing);

        const laserLens = new Mesh(new CylinderGeometry(0.01, 0.06, 0.08, 16), laserMat);
        laserLens.rotation.x = -Math.PI / 2;
        laserLens.position.set(0, 0.12, -0.44);
        this.add(laserLens);

        this.add(this.createUltrasoundModule(1, micMat, darkMat, sensorMat));
        this.add(this.createUltrasoundModule(-1, micMat, darkMat, sensorMat));
    }

    private createUltrasoundModule(side: -1 | 1, micMat: MeshStandardMaterial, darkMat: MeshStandardMaterial, sensorMat: MeshStandardMaterial): Group {
        const module = new Group();
        const angle = side * Math.PI / 3;
        module.rotation.y = angle;
        module.position.set(-1*side * 0.40, 0.20, -0.25);

        const bracket = new Mesh(new BoxGeometry(0.3, 0.14, 0.12), darkMat);
        bracket.position.set(0, 0, 0);
        module.add(bracket);

        const emitterLeft = new Mesh(new CylinderGeometry(0.05, 0.05, 0.08, 18), sensorMat);
        emitterLeft.rotation.x = Math.PI / 2;
        emitterLeft.position.set(-0.07, 0.01, -0.08);
        module.add(emitterLeft);

        const emitterRight = new Mesh(new CylinderGeometry(0.05, 0.05, 0.08, 18), sensorMat);
        emitterRight.rotation.x = Math.PI / 2;
        emitterRight.position.set(0.07, 0.01, -0.08);
        module.add(emitterRight);

        const microphoneBase = new Mesh(new CylinderGeometry(0.04, 0.04, 0.06, 14), micMat);
        microphoneBase.position.set(0, 0.12, -0.01);
        module.add(microphoneBase);

        const microphoneCap = new Mesh(new SphereGeometry(0.035, 16, 12), micMat);
        microphoneCap.position.set(0, 0.17, -0.01);
        module.add(microphoneCap);

        return module;
    }

    addProximityCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.PROXIMITY, ClusterProximityCommands.US_LEFT, (message: Message) => {
            if (message.params) {
                this.sensors.proximity.left = message.getValueUint16(0);
                this.proximity.show(this.sensors.proximity.left, ProximitySide.left);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.PROXIMITY, ClusterProximityCommands.US_RIGHT, (message: Message) => {
            if (message.params) {
                this.sensors.proximity.right = message.getValueUint16(0);
                this.proximity.show(this.sensors.proximity.right, ProximitySide.right);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.PROXIMITY, ClusterProximityCommands.LASER, (message: Message) => {
            if (message.params) {
                for (let i = 0; i < 8; i++) {
                    for (let j = 0; j < 8; j++) {
                        this.sensors.proximity.front[i][j] = message.getValueUint16((i * 8 + j) * 2) / 10;
                    }
                }
                this.proximity.show(this.sensors.proximity.front, ProximitySide.center);
            }
        });
    }

    addSoundCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.SOUND, ClusterSoundCommands.SOUND_STATUS, (message: Message) => {
            if (message.params) {
                if (message.getValueUint8(0) == 0) {
                    this.sensors.sound.left = message.getValueUint24(1);
                    let val = Math.min(Math.max(Math.ceil(this.sensors.sound.right / 2000), 1), 3);
                    this.sound.show(val, SoundSide.left);
                } else {
                    this.sensors.sound.right = message.getValueUint24(1);
                    let val = Math.min(Math.max(Math.ceil(this.sensors.sound.right / 2000), 1), 3);
                    this.sound.show(val, SoundSide.right);
                }
            }
        });
    }

    update() {
        this.proximity.update();
    }
}
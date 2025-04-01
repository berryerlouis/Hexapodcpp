import Socket from "../../communication/socket.ts";
import Message from "../../communication/message.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterProximityCommands} from "../../communication/clusters/clusterProximity.ts";
import {ClusterSoundCommands} from "../../communication/clusters/clusterSound.ts";
import SoundObject, {SoundSide} from "./sound.ts";
import {Object3D} from "three";
import ProximityObject, {ProximitySide} from "./proximity.ts";

interface Proximity {
    left: number;
    front: number;
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
    sensors:Sensors = {
        proximity: {
            left: 0,
            right: 0,
            front: 0
        },
        sound: {
            left: 0,
            right: 0
        }
    }
    socket:Socket;
    sound:SoundObject;
    proximity:ProximityObject;

    constructor(socket: Socket) {
        super();
        this.socket = socket;
        this.sound = new SoundObject();
        this.proximity = new ProximityObject();

        this.add(this.sound);
        this.add(this.proximity);

        this.addProximityCallbacks();
        this.addSoundCallbacks();
    }

    addProximityCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.PROXIMITY, ClusterProximityCommands.US_LEFT ,(message:Message) => {
            if(message.params) {
                this.sensors.proximity.left = message.params[0] + (message.params[1] << 8);
                this.proximity.show(this.sensors.proximity.left, ProximitySide.left);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.PROXIMITY, ClusterProximityCommands.US_RIGHT ,(message:Message) => {
            if(message.params) {
                this.sensors.proximity.right = message.params[0] + (message.params[1] << 8);
                this.proximity.show(this.sensors.proximity.right, ProximitySide.right);
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.PROXIMITY, ClusterProximityCommands.LASER ,(message:Message) => {
            if(message.params) {
                this.sensors.proximity.front = (message.params[0] + (message.params[1] << 8)) / 10;
                this.proximity.show(this.sensors.proximity.front, ProximitySide.center);
            }
        });
    }
    addSoundCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.SOUND, ClusterSoundCommands.SOUND_STATUS ,(message:Message) => {
            if(message.params)
                if(message.params[0] == 0) {
                    this.sensors.sound.left = message.params[1] +
                        (message.params[2] << 8) +
                        (message.params[3] << 16) +
                        (message.params[4] << 24);
                    let val = Math.min(Math.max(Math.ceil(this.sensors.sound.right / 2000), 1), 3);
                    this.sound.show(val, SoundSide.left);
                }
                else {
                    this.sensors.sound.right = message.params[1] +
                        (message.params[2] << 8) +
                        (message.params[3] << 16) +
                        (message.params[4] << 24);
                    let val = Math.min(Math.max(Math.ceil(this.sensors.sound.right / 2000), 1), 3);
                    this.sound.show(val, SoundSide.right);
                }
        });
    }

    update() {
        this.proximity.update();
    }
}
import {FolderApi, Pane} from 'tweakpane';
import Hexapod from "../entity/hexapod.ts";
import Socket from "../communication/socket.ts";
import Message from "../communication/message.ts";
import {ClusterGenericCommands, ClusterName} from "../communication/clusters/clusterType.ts";
import {ClusterSoundCommands} from "../communication/clusters/clusterSound.ts";
import {ClusterProximityCommands} from "../communication/clusters/clusterProximity.ts";
import {ClusterServoCommands} from "../communication/clusters/clusterServo.ts";
import {ClusterGeneralCommands} from "../communication/clusters/clusterGeneral.ts";
import {ClusterImuCommands} from "../communication/clusters/clusterImu.ts";
import {ClusterBodyCommands} from "../communication/clusters/clusterBody.ts";
import DirectionArrow from "./directionArrow.ts";

export default class Panel extends Pane {
    hexapod:Hexapod;
    directionArrow:DirectionArrow;
    socket:Socket;
    initDone:boolean;
    constructor(domElement:HTMLElement|undefined, hexapod:Hexapod, directionArrow:DirectionArrow, socket:Socket) {
        super({container: domElement});

        this.hexapod = hexapod;
        this.directionArrow = directionArrow;
        this.socket = socket;
        this.initDone = false;

        this.setCallbackVersion();

        const mainFolder = this.addFolder({
            title: 'Hexapod',
            expanded: true,
        });

        this.buildBatteryFolder(mainFolder);
        this.buildImuFolder(mainFolder);

        const bodyFolder = mainFolder.addFolder({
            title: 'Body',
            expanded: false,
        });
        this.buildPositionFolder(bodyFolder);
        this.buildRotationFolder(bodyFolder);
        this.buildDirectionFolder(bodyFolder);
        this.buildAmplitudeElevationFolder(bodyFolder);
        this.buildLegsFolder(bodyFolder);

        const controlFolder = mainFolder.addFolder({
            title: 'Control',
            expanded: false,
        });
        this.buildHeadFolder(controlFolder);
        this.buildServoFolder(controlFolder);

        /*b.on('change', function(ev) {
            hexapod.angle = ev.value;
        });*/

        /*this.addBinding(hexapod, 'signal', {
            readonly: true,
            view: 'graph',
            interval: 200,
            min: -1,
            max: +1,
        });*/
    }
    update() {
        this.refresh()
    }
    setCallbackVersion () {
        this.socket.addSpecificCallbackRead(ClusterName.GENERAL, ClusterGeneralCommands.VERSION, () => {
            this.initDone = true;
        });
    }

    buildServoFolder(folder:FolderApi) {
        const servosLegsFolder: FolderApi[] = [];
        const servoLegFolder: any[] = [];
        const servosFolder = folder.addFolder({
            title: 'Servos',
            expanded: false,
        });
        servosFolder.addBinding(this.hexapod.body.members.legs, 'status', {label:'enable'}).on('change', (ev) => {
            if (this.initDone) {
                if (!ev.value) {
                    this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, [0]));
                } else {
                    this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, [1]));
                }
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_STATE_PCA, (message:Message) => {
            if(message.params) {
                for (let i = 0; i < 6; i++) {
                    servosLegsFolder[i].disabled = !message.getValueBool(0);
                }
            }
        });
        for (let i = 0; i < 6; i++) {
            servosLegsFolder[i] = servosFolder.addFolder({
                title: this.hexapod.body.members.legs.leg[i].legData.name,
                expanded: false,
            });
            servosLegsFolder[i].disabled = true;

            for (let j = 0; j < 3; j++) {
                let servoId = i * 3 + j;
                servoLegFolder[servoId] = [];
                const servoLegsFolder = servosLegsFolder[i].addFolder({
                    title: this.hexapod.body.members.legs.leg[i].legData.servos[j].name,
                    expanded: false,
                });

                this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_STATE, (message:Message) => {
                    servoLegFolder[message.getValueUint8(0)]['angle'].disabled = !message.getValueBool(1);
                    servoLegFolder[message.getValueUint8(0)]['angle-graph'].disabled = !message.getValueBool(1);
                    servoLegFolder[message.getValueUint8(0)]['min'].disabled = !message.getValueBool(1);
                    servoLegFolder[message.getValueUint8(0)]['max'].disabled = !message.getValueBool(1);
                },[servoId]);
                this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.SET_STATE, (message:Message) => {
                    if(message.params) {
                        servoLegFolder[message.getValueUint8(0)]['angle'].disabled = !message.getValueBool(1);
                        servoLegFolder[message.getValueUint8(0)]['angle-graph'].disabled = !message.getValueBool(1);
                        servoLegFolder[message.getValueUint8(0)]['min'].disabled = !message.getValueBool(1);
                        servoLegFolder[message.getValueUint8(0)]['max'].disabled = !message.getValueBool(1);
                    }
                },[servoId]);

                this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterGenericCommands.GENERIC, (message:Message) => {
                    if(message.params) {
                        if (message.getValueUint8(0) == servoId) {
                            this.hexapod.body.members.legs.leg[i].legData.servos[j].angle = message.getValueUint8(2);
                        }
                    }
                },[servoId]);
                servoLegFolder[servoId]['status'] = servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'status').on('change', (ev) => {
                    if (this.initDone) {
                        if (!ev.value) {
                            this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE, [servoId, 0]));
                        } else {
                            this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_STATE, [servoId, 1]));
                        }
                    }
                });

                servoLegFolder[servoId]['min'] = servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'min', {min: 0, max: 180, step: 1
                }).on('change', (ev) => {
                    if (this.initDone && ev.last) {
                        if (ev.last) {
                            this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_MIN, [servoId, ev.value]));
                        }
                    }
                });

                servoLegFolder[servoId]['max'] = servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'max', {min: 0, max: 180, step: 1
                }).on('change', (ev) => {
                    if (this.initDone && ev.last) {
                        if (ev.last) {
                            this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_MAX, [servoId, ev.value]));
                        }
                    }
                });

                servoLegFolder[servoId]['angle'] = servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'angle', {min: 0, max: 180, step: 1
                }).on('change', (ev) => {
                    if (this.initDone && ev.last && this.hexapod.body.members.legs.leg[i].legData.servos[j].status) {
                        if (ev.last) {
                            //this.socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.SET_ANGLE, [servoId, ev.value]));
                        }
                    }
                });

                servoLegFolder[servoId]['angle-graph'] = servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'angle',{
                    readonly: true,
                    view: 'graph',
                    min: 0,
                    max: 180,
                });
            }
        }
    }

    buildHeadFolder(folder:FolderApi) {
        const proximityFolder = folder.addFolder({
            title: 'Proximity',
            expanded: false,
        });
        this.buildProximityFolder(proximityFolder);

        const soundFolder = folder.addFolder({
            title: 'Sound',
            expanded: false,
        });
        this.buildSoundFolder(soundFolder);
    }

    buildProximityFolder(folder:FolderApi) {
        const usLeft = folder.addFolder({
            title: 'Left',
            expanded: false,
        });
        const laserFront = folder.addFolder({
            title: 'Front',
            expanded: false,
        });
        const usRight = folder.addFolder({
            title: 'Right',
            expanded: false,
        });
        usLeft.addBinding(this.hexapod.head.sensors.proximity, 'left', {readonly: true});
        usLeft.addButton({
            title: 'Get',
            label: 'status',   // optional
        }).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message( ClusterName.PROXIMITY, ClusterProximityCommands.US_LEFT));
            }
        });
        laserFront.addBinding(this.hexapod.head.sensors.proximity, 'front', {readonly: true});
        laserFront.addButton({
            title: 'Get',
            label: 'status',   // optional
        }).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message( ClusterName.PROXIMITY, ClusterProximityCommands.LASER));
            }
        });
        usRight.addBinding(this.hexapod.head.sensors.proximity, 'right', {readonly: true});
        usRight.addButton({
            title: 'Get',
            label: 'status',   // optional
        }).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message( ClusterName.PROXIMITY, ClusterProximityCommands.US_RIGHT));
            }
        });
    }

    buildSoundFolder(folder:FolderApi) {
        const usLeft = folder.addFolder({
            title: 'Left',
            expanded: false,
        });
        const usRight = folder.addFolder({
            title: 'Right',
            expanded: false,
        });
        usLeft.addBinding(this.hexapod.head.sensors.sound, 'left', {readonly: true});
        usLeft.addButton({
            title: 'Get',
            label: 'status',   // optional
        }).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message( ClusterName.SOUND, ClusterSoundCommands.SOUND_STATUS, [0]));
            }
        });
        usRight.addBinding(this.hexapod.head.sensors.sound, 'right', {readonly: true});
        usRight.addButton({
            title: 'Get',
            label: 'status',   // optional
        }).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message( ClusterName.SOUND, ClusterSoundCommands.SOUND_STATUS, [1]));
            }
        });
    }

    buildPositionFolder(folder: FolderApi) {
        const bodyPositionFolder = folder.addFolder({
            title: 'Position',
            expanded: false,
        });
        bodyPositionFolder.addBinding(this.hexapod.hexapodStruct.position, 'x', {min: -30, max: 30, step: 1}).on('change',this.sendBodyPositionRotation.bind(this));
        bodyPositionFolder.addBinding(this.hexapod.hexapodStruct.position, 'y', {min: -30, max: 30, step: 1}).on('change',this.sendBodyPositionRotation.bind(this));
        bodyPositionFolder.addBinding(this.hexapod.hexapodStruct.position, 'z', {min: -30, max: 30, step: 1}).on('change',this.sendBodyPositionRotation.bind(this));
    }

    buildRotationFolder(folder: FolderApi) {
        const bodyRotationFolder = folder.addFolder({
            title: 'Rotation',
            expanded: false,
        });
        bodyRotationFolder.addBinding(this.hexapod.hexapodStruct.rotation, 'x', {min: -30, max: 30, step: 1}).on('change',this.sendBodyPositionRotation.bind(this));
        bodyRotationFolder.addBinding(this.hexapod.hexapodStruct.rotation, 'y', {min: -30, max: 30, step: 1}).on('change',this.sendBodyPositionRotation.bind(this));
        bodyRotationFolder.addBinding(this.hexapod.hexapodStruct.rotation, 'z', {min: -30, max: 30, step: 1}).on('change',this.sendBodyPositionRotation.bind(this));
    }

    buildLegsFolder(folder: FolderApi) {
        const bodyLegsFolder = folder.addFolder({
            title: 'Legs',
            expanded: false,
        });

        for (let i = 0; i < 6; i++) {
            const bodyLegFolder = bodyLegsFolder.addFolder({
                title: this.hexapod.body.members.legs.leg[i].legData.name,
                expanded: false,
            });

            if(i == 0) {
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'x', {min: -30, max: 30, step: 1}).on('change',this.sendLeg0Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'y', {min: -30, max: 30, step: 1}).on('change',this.sendLeg0Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'z', {min: -30, max: 30, step: 1}).on('change',this.sendLeg0Position.bind(this));
            } else if(i == 1) {
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'x', {min: -30, max: 30, step: 1}).on('change',this.sendLeg1Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'y', {min: -30, max: 30, step: 1}).on('change',this.sendLeg1Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'z', {min: -30, max: 30, step: 1}).on('change',this.sendLeg1Position.bind(this));
            } else if(i == 2) {
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'x', {min: -30, max: 30, step: 1}).on('change',this.sendLeg2Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'y', {min: -30, max: 30, step: 1}).on('change',this.sendLeg2Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'z', {min: -30, max: 30, step: 1}).on('change',this.sendLeg2Position.bind(this));
            } else if(i == 3) {
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'x', {min: -30, max: 30, step: 1}).on('change',this.sendLeg3Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'y', {min: -30, max: 30, step: 1}).on('change',this.sendLeg3Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'z', {min: -30, max: 30, step: 1}).on('change',this.sendLeg3Position.bind(this));
            } else if(i == 4) {
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'x', {min: -30, max: 30, step: 1}).on('change',this.sendLeg4Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'y', {min: -30, max: 30, step: 1}).on('change',this.sendLeg4Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'z', {min: -30, max: 30, step: 1}).on('change',this.sendLeg4Position.bind(this));
            } else if(i == 5) {
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'x', {min: -30, max: 30, step: 1}).on('change',this.sendLeg5Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'y', {min: -30, max: 30, step: 1}).on('change',this.sendLeg5Position.bind(this));
                bodyLegFolder.addBinding(this.hexapod.body.members.legs.leg[i], 'z', {min: -30, max: 30, step: 1}).on('change',this.sendLeg5Position.bind(this));
            }
        }
    }

    sendBodyPositionRotation (ev:any) {
        if (this.initDone && ev.last) {
             this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_BODY_X_Y_Z,
                 [
                     ev.target.element.parentElement.parentElement.outerText.includes('Position') && ev.target.key == 'x' ? Math.floor(ev.value) : this.hexapod.hexapodStruct.position.x,
                     ev.target.element.parentElement.parentElement.outerText.includes('Position') && ev.target.key == 'y' ? Math.floor(ev.value) : this.hexapod.hexapodStruct.position.y,
                     ev.target.element.parentElement.parentElement.outerText.includes('Position') && ev.target.key == 'z' ? Math.floor(ev.value) : this.hexapod.hexapodStruct.position.z,
                     ev.target.element.parentElement.parentElement.outerText.includes('Rotation') && ev.target.key == 'x' ? Math.floor(ev.value) : this.hexapod.hexapodStruct.rotation.x,
                     ev.target.element.parentElement.parentElement.outerText.includes('Rotation') && ev.target.key == 'y' ? Math.floor(ev.value) : this.hexapod.hexapodStruct.rotation.y,
                     ev.target.element.parentElement.parentElement.outerText.includes('Rotation') && ev.target.key == 'z' ? Math.floor(ev.value) : this.hexapod.hexapodStruct.rotation.z,
                     1000
                 ],
                 [
                     0xFFFF,
                     0xFFFF,
                     0xFFFF,
                     0xFFFF,
                     0xFFFF,
                     0xFFFF,
                     0xFFFF,
                 ]));
         }
    }

    sendLeg0Position (ev:any) {
        this.sendLegPosition(ev, 0);
    }
    sendLeg1Position (ev:any) {
        this.sendLegPosition(ev, 1);
    }
    sendLeg2Position (ev:any) {
        this.sendLegPosition(ev, 2);
    }
    sendLeg3Position (ev:any) {
        this.sendLegPosition(ev, 3);
    }
    sendLeg4Position (ev:any) {
        this.sendLegPosition(ev, 4);
    }
    sendLeg5Position (ev:any) {
        this.sendLegPosition(ev, 5);
    }
    sendLegPosition (ev:any, legIndex:number) {
        if (this.initDone && ev.last) {
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_LEG_X_Y_Z,
                [
                    legIndex,
                    ev.target.key == 'x' ? Math.floor(ev.value) : Math.floor(this.hexapod.body.members.legs.leg[legIndex].x),
                    ev.target.key == 'y' ? Math.floor(ev.value) : Math.floor(this.hexapod.body.members.legs.leg[legIndex].y),
                    ev.target.key == 'z' ? Math.floor(ev.value) : Math.floor(this.hexapod.body.members.legs.leg[legIndex].z),
                    1000
                ],
                [
                    0xFF,
                    0xFFFF,
                    0xFFFF,
                    0xFFFF,
                    0xFFFF,
                ]));
        }
    }

    buildBatteryFolder(folder: FolderApi) {
        const batteryFolder = folder.addFolder({
            title: 'Battery',
            expanded: false,
        });
        batteryFolder.addBinding(this.hexapod.battery.batteryData, 'voltage',{
            readonly: true
        });
        batteryFolder.addBinding(this.hexapod.battery.batteryData, 'current',{
            readonly: true
        });
        batteryFolder.addBinding(this.hexapod.battery.batteryData, 'voltage',{
            readonly: true,
            view: 'graph',
            min: 0,
            max: 10,
        });
        batteryFolder.addBinding(this.hexapod.battery.batteryData, 'current',{
            readonly: true,
            view: 'graph',
            min: 0,
            max: 1,
        });
    }

    buildImuFolder(folder: FolderApi) {
        const imuFolder = folder.addFolder({
            title: 'Imu',
            expanded: false,
        });
        const calibFolder = imuFolder.addFolder({
            title: 'Calibration',
            expanded: false,
        });
        calibFolder.addButton({ title: 'Start', label: 'acc'}).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message( ClusterName.IMU, ClusterImuCommands.START_CALIB, [1,1]));
            }
        });
        calibFolder.addButton({ title: 'Start', label: 'gyro'}).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message( ClusterName.IMU, ClusterImuCommands.START_CALIB, [2,1]));
            }
        });
        calibFolder.addButton({ title: 'Start', label: 'mag'}).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message( ClusterName.IMU, ClusterImuCommands.START_CALIB, [4,1]));
            }
        });

        const temperatureFolder = imuFolder.addFolder({
            title: 'Temperature',
            expanded: false,
        });
        temperatureFolder.addBinding(this.hexapod.imu.imuData, 'temperature',{
            readonly: true,
            view: 'graph',
            min: 0,
            max: 30,
        });
        const yprFolder = imuFolder.addFolder({
            title: 'YawPitchRoll',
            expanded: false,
        });
        yprFolder.addBinding(this.hexapod.imu.imuData.ypr, 'roll',{
            readonly: true,
        });
        yprFolder.addBinding(this.hexapod.imu.imuData.ypr, 'pitch',{
            readonly: true,
        });
        yprFolder.addBinding(this.hexapod.imu.imuData.ypr, 'yaw',{
            readonly: true,
        });

        const accFolder = imuFolder.addFolder({
            title: 'Acc',
            expanded: false,
        });
        accFolder.addBinding(this.hexapod.imu.imuData.accel, 'x',{
            readonly: true, view: 'graph', min: -180, max: 180,
        });
        accFolder.addBinding(this.hexapod.imu.imuData.accel, 'y',{
            readonly: true, view: 'graph', min: -180, max: 180,
        });
        accFolder.addBinding(this.hexapod.imu.imuData.accel, 'z',{
            readonly: true, view: 'graph', min: -180, max: 180,
        });

        const gyrFolder = imuFolder.addFolder({
            title: 'Gyro',
            expanded: false,
        });
        gyrFolder.addBinding(this.hexapod.imu.imuData.gyro, 'x',{
            readonly: true, view: 'graph', min: -4000, max: 4000,
        });
        gyrFolder.addBinding(this.hexapod.imu.imuData.gyro, 'y',{
            readonly: true, view: 'graph', min: -4000, max: 4000,
        });
        gyrFolder.addBinding(this.hexapod.imu.imuData.gyro, 'z',{
            readonly: true, view: 'graph', min: -4000, max: 4000,
        });

        const magFolder = imuFolder.addFolder({
            title: 'Mag',
            expanded: false,
        });
        magFolder.addBinding(this.hexapod.imu.imuData.mag, 'x',{
            readonly: true, view: 'graph', min: -180, max: 180,
        });
        magFolder.addBinding(this.hexapod.imu.imuData.mag, 'y',{
            readonly: true, view: 'graph', min: -180, max: 180,
        });
        magFolder.addBinding(this.hexapod.imu.imuData.mag, 'z',{
            readonly: true, view: 'graph', min: -180, max: 180,
        });
    }

    buildDirectionFolder(folder: FolderApi) {

        const bodyDirectionFolder = folder.addFolder({
            title: 'Direction',
            expanded: false,
        });
        bodyDirectionFolder.addBinding(this.hexapod.hexapodStruct, 'direction', {min: 0, max: 360, step: 1}).on('change',(ev)=> {
            if (this.initDone && ev.last) {
                this.directionArrow.setDirection(ev.value);
                this.hexapod.setDirection(ev.value);
                this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_DIRECTION,
                    [ev.value],[0xFFFF]));
            }
        });
    }

    buildAmplitudeElevationFolder(folder: FolderApi) {

        const bodyAmplitudeFolder = folder.addFolder({
            title: 'Amplitude',
            expanded: false,
        });
        bodyAmplitudeFolder.addBinding(this.hexapod.hexapodStruct, 'amplitude', {min: 0, max: 30, step: 1}).on('change',(ev)=> {
            if (this.initDone && ev.last) {
                this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_AMPLITUDE_ELEVATION,
                    [
                        ev.value,
                        this.hexapod.hexapodStruct.elevation
                    ]));
            }
        });
        const bodyElevationFolder = folder.addFolder({
            title: 'Elevation',
            expanded: false,
        });
        bodyElevationFolder.addBinding(this.hexapod.hexapodStruct, 'elevation', {min: 0, max: 30, step: 1}).on('change',(ev)=> {
            if (this.initDone && ev.last) {
                this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_AMPLITUDE_ELEVATION,
                    [
                        this.hexapod.hexapodStruct.amplitude,
                        ev.value,
                    ]));
            }
        });
    }
}

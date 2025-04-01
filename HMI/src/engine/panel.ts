import {FolderApi, Pane} from 'tweakpane';
import Hexapod from "../entity/hexapod.ts";
import Socket from "../communication/socket.ts";
import Message from "../communication/message.ts";
import {Direction} from "../communication/protocol.ts";
import {ClusterGenericCommands, ClusterName} from "../communication/clusters/clusterType.ts";
import {ClusterSoundCommands} from "../communication/clusters/clusterSound.ts";
import {ClusterProximityCommands} from "../communication/clusters/clusterProximity.ts";
import {ClusterServoCommands} from "../communication/clusters/clusterServo.ts";
import {ClusterGeneralCommands} from "../communication/clusters/clusterGeneral.ts";
import {ClusterImuCommands} from "../communication/clusters/clusterImu.ts";

export default class Panel extends Pane {
    hexapod:Hexapod;
    socket:Socket;
    initDone:boolean;
    constructor(domElement:HTMLElement|undefined, hexapod:Hexapod, socket:Socket) {
        super({container: domElement});

        this.hexapod = hexapod;
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
                    this.socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, 1, [0]));
                } else {
                    this.socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, 1, [1]));
                }
            }
        });
        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_STATE_PCA, (message:Message) => {
            if(message.params) {
                for (let i = 0; i < 6; i++) {
                    servosLegsFolder[i].disabled = message.params[0] == 0;
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
                    if(message.params) {
                        servoLegFolder[message.params[0]]['angle'].disabled = message.params[1]==0;
                        servoLegFolder[message.params[0]]['angle-graph'].disabled = message.params[1]==0;
                        servoLegFolder[message.params[0]]['min'].disabled = message.params[1]==0;
                        servoLegFolder[message.params[0]]['max'].disabled = message.params[1]==0;
                    }
                },[servoId]);
                this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.SET_STATE, (message:Message) => {
                    if(message.params) {
                        servoLegFolder[message.params[0]]['angle'].disabled = message.params[1]==0;
                        servoLegFolder[message.params[0]]['angle-graph'].disabled = message.params[1]==0;
                        servoLegFolder[message.params[0]]['min'].disabled = message.params[1]==0;
                        servoLegFolder[message.params[0]]['max'].disabled = message.params[1]==0;
                    }
                },[servoId]);

                this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterGenericCommands.GENERIC, (message:Message) => {
                    if(message.params) {
                        if (message.params[0] == servoId) {
                            this.hexapod.body.members.legs.leg[i].legData.servos[j].angle = message.params[2];
                        }
                    }
                },[servoId]);
                servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'status').on('change', (ev) => {
                    if (this.initDone) {
                        if (!ev.value) {
                            this.socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.SET_STATE, 2, [servoId, 0]));
                        } else {
                            this.socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.SET_STATE, 2, [servoId, 1]));
                        }
                    }
                });

                servoLegFolder[servoId]['angle'] = servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'angle', {min: 0, max: 180, step: 1
                }).on('change', (ev) => {
                    if (this.initDone && this.hexapod.body.members.legs.leg[i].legData.servos[j].status) {
                        if (ev.last) {
                            this.socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.SET_ANGLE, 2, [servoId, ev.value]));
                        }
                    }
                });

                servoLegFolder[servoId]['angle-graph'] = servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'angle',{
                    readonly: true,
                    view: 'graph',
                    min: 0,
                    max: 180,
                });

                servoLegFolder[servoId]['min'] = servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'min', {min: 0, max: 180, step: 1
                }).on('change', (ev) => {
                    if (this.initDone) {
                        if (ev.last) {
                            this.socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.SET_MIN, 2, [servoId, ev.value]));
                        }
                    }
                });

                servoLegFolder[servoId]['max'] = servoLegsFolder.addBinding(this.hexapod.body.members.legs.leg[i].legData.servos[j], 'max', {min: 0, max: 180, step: 1
                }).on('change', (ev) => {
                    if (this.initDone) {
                        if (ev.last) {
                            this.socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.SET_MAX, 2, [servoId, ev.value]));
                        }
                    }
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
                this.socket.write(new Message(Direction.TX, ClusterName.PROXIMITY, ClusterProximityCommands.US_LEFT));
            }
        });
        laserFront.addBinding(this.hexapod.head.sensors.proximity, 'front', {readonly: true});
        laserFront.addButton({
            title: 'Get',
            label: 'status',   // optional
        }).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message(Direction.TX, ClusterName.PROXIMITY, ClusterProximityCommands.LASER));
            }
        });
        usRight.addBinding(this.hexapod.head.sensors.proximity, 'right', {readonly: true});
        usRight.addButton({
            title: 'Get',
            label: 'status',   // optional
        }).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message(Direction.TX, ClusterName.PROXIMITY, ClusterProximityCommands.US_RIGHT));
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
                this.socket.write(new Message(Direction.TX, ClusterName.SOUND, ClusterSoundCommands.SOUND_STATUS, 1, [0]));
            }
        });
        usRight.addBinding(this.hexapod.head.sensors.sound, 'right', {readonly: true});
        usRight.addButton({
            title: 'Get',
            label: 'status',   // optional
        }).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message(Direction.TX, ClusterName.SOUND, ClusterSoundCommands.SOUND_STATUS, 1, [1]));
            }
        });
    }

    buildPositionFolder(folder: FolderApi) {
        const bodyPositionFolder = folder.addFolder({
            title: 'Position',
            expanded: false,
        });
        bodyPositionFolder.addBinding(this.hexapod.hexapodStruct.position, 'x', {min: -1, max: 1, step: 0.01});
        bodyPositionFolder.addBinding(this.hexapod.hexapodStruct.position, 'y', {min: -1, max: 1, step: 0.01});
        bodyPositionFolder.addBinding(this.hexapod.hexapodStruct.position, 'z', {min: -1, max: 1, step: 0.01});
    }

    buildRotationFolder(folder: FolderApi) {
        const bodyRotationFolder = folder.addFolder({
            title: 'Rotation',
            expanded: false,
        });
        bodyRotationFolder.addBinding(this.hexapod.hexapodStruct.rotation, 'x', {min: -Math.PI/4, max: Math.PI/4, step: 0.01});
        bodyRotationFolder.addBinding(this.hexapod.hexapodStruct.rotation, 'y', {min: -Math.PI/4, max: Math.PI/4, step: 0.01});
        bodyRotationFolder.addBinding(this.hexapod.hexapodStruct.rotation, 'z', {min: -Math.PI/4, max: Math.PI/4, step: 0.01});
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
                this.socket.write(new Message(Direction.TX, ClusterName.IMU, ClusterImuCommands.START_CALIB, 2, [1,1]));
            }
        });
        calibFolder.addButton({ title: 'Start', label: 'gyro'}).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message(Direction.TX, ClusterName.IMU, ClusterImuCommands.START_CALIB, 2, [2,1]));
            }
        });
        calibFolder.addButton({ title: 'Start', label: 'mag'}).on('click', () => {
            if (this.initDone) {
                this.socket.write(new Message(Direction.TX, ClusterName.IMU, ClusterImuCommands.START_CALIB, 2, [4,1]));
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
}

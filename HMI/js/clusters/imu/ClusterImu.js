import {ClusterName, CommandImu} from "../../protocol/Cluster.js";

export class ClusterImu {
    constructor(messageManager, databaseManager, robot, compass) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;
        this.compass = compass;
        this.initialize();

        this.pressureHMI = $('#hexapod-pressure');
        this.temperatureHMI = $('#hexapod-temperature');
        this.altitudeHMI = $('#hexapod-altitude');
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.ALL, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'ACC',
                    item: 'x',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'ACC',
                    item: 'y',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'ACC',
                    item: 'z',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'GYR',
                    item: 'x',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'GYR',
                    item: 'y',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'GYR',
                    item: 'z',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'MAG',
                    item: 'x',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'MAG',
                    item: 'y',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'MAG',
                    item: 'z',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'TMP',
                    value: message.fetchInt16U() + ' °C'
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.ACC, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'ACC',
                    item: 'x',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'ACC',
                    item: 'y',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'ACC',
                    item: 'z',
                    value: message.fetchInt16S()
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.GYR, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'GYR',
                    item: 'x',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'GYR',
                    item: 'y',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'GYR',
                    item: 'z',
                    value: message.fetchInt16S()
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.MAG, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'MAG',
                    item: 'x',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'MAG',
                    item: 'y',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'MAG',
                    item: 'z',
                    value: message.fetchInt16S()
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.TMP, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'TMP',
                    value: message.fetchInt16U() + ' °C'
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.YAWPITCHROLL, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'YAWPITCHROLL',
                    item: 'roll',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'YAWPITCHROLL',
                    item: 'pitch',
                    value: message.fetchInt16S()
                });
                let angle = message.fetchInt16S();
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'YAWPITCHROLL',
                    item: 'yaw',
                    value: angle
                });
                this.compass.rotate(angle);
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.PRESSURE, (message) => {
            if (message.size > 0) {
                let val = (message.fetchInt32U() / 10).toFixed(1) + ' pa';
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'PRESSURE',
                    value: val
                });
                this.pressureHMI[0].innerText = val;
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.ALTITUDE, (message) => {
            if (message.size > 0) {
                let val = message.fetchInt16U() + ' m';
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'ALTITUDE',
                    value: val
                });
                this.altitudeHMI[0].innerText = val;
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.TMPBAR, (message) => {

            if (message.size > 0) {
                let val = (message.fetchInt16S() / 100).toFixed(2) + ' °C';
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'TMPBAR',
                    value: val
                });
                this.temperatureHMI[0].innerText = val;
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.CALIB_MAG_MIN_MAX, (message) => {
            if (message.size > 0) {
                let min = message.fetchInt8U();
                let x = (message.fetchInt32S() / 100).toFixed(2);
                let y = (message.fetchInt32S() / 100).toFixed(2);
                let z = (message.fetchInt32S() / 100).toFixed(2);
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: min === 0 ? 'CALIB_MAG_MIN' : 'CALIB_MAG_MAX',
                    item: 'x',
                    value: x
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: min === 0 ? 'CALIB_MAG_MIN' : 'CALIB_MAG_MAX',
                    item: 'y',
                    value: y
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: min === 0 ? 'CALIB_MAG_MIN' : 'CALIB_MAG_MAX',
                    item: 'z',
                    value: z
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.START_STOP_MAG_CALIB, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'STARTSTOPMAGCALIB',
                    value: message.fetchInt8U() === 1
                });
            }
        });
    }
}


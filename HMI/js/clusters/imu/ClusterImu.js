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
                    item: 'x',
                    value: message.fetchInt16S()
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'YAWPITCHROLL',
                    item: 'y',
                    value: message.fetchInt16S()
                });
                let angle = message.fetchInt16S();
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'YAWPITCHROLL',
                    item: 'z',
                    value: angle
                });
                this.compass.rotate(angle);
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.PRESSURE, (message) => {
            if (message.size > 0) {
                let val = message.fetchInt32U() + ' pa';
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
    }
}


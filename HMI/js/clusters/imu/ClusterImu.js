import {ClusterName, CommandImu} from "../../protocol/Cluster.js";

export class ClusterImu {
    constructor(messageManager, databaseManager, robot) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.ALL, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'ACC',
                    value: {x: message.fetchInt16S(), y: message.fetchInt16S(), z: message.fetchInt16S()}
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'GYR',
                    value: {x: message.fetchInt16S(), y: message.fetchInt16S(), z: message.fetchInt16S()}
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'MAG',
                    value: {x: message.fetchInt16S(), y: message.fetchInt16S(), z: message.fetchInt16S()}
                });
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'TMP',
                    value: message.fetchInt16U()
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.ACC, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'ACC',
                    value: {x: message.fetchInt16S(), y: message.fetchInt16S(), z: message.fetchInt16S()}
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.GYR, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'GYR',
                    value: {x: message.fetchInt16S(), y: message.fetchInt16S(), z: message.fetchInt16S()}
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.MAG, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'MAG',
                    value: {x: message.fetchInt16S(), y: message.fetchInt16S(), z: message.fetchInt16S()}
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.IMU, CommandImu.TMP, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'IMU',
                    command: 'TMP',
                    value: message.fetchInt16U()
                });
            }
        });
    }
}


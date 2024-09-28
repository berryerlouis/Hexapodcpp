import {ClusterName, CommandProximity} from "../../protocol/Cluster.js";

export class ClusterProximity {
    constructor(messageManager, databaseManager, robot) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.PROXIMITY, CommandProximity.LASER, (message) => {
            if (message.size > 0) {
                let distance = message.fetchInt16U();
                this.robot.drawObstacleCenter(distance);
                this.databaseManager.updateDb({
                    cluster: 'PROXIMITY',
                    command: 'LASER',
                    value: distance
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.PROXIMITY, CommandProximity.US_LEFT, (message) => {
            if (message.size > 0) {
                let distance = message.fetchInt16U();
                this.robot.drawObstacleLeft(distance);
                this.databaseManager.updateDb({
                    cluster: 'PROXIMITY',
                    command: 'US_LEFT',
                    value: distance
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.PROXIMITY, CommandProximity.US_RIGHT, (message) => {
            if (message.size > 0) {
                let distance = message.fetchInt16U();
                this.robot.drawObstacleRight(distance);
                this.databaseManager.updateDb({
                    cluster: 'PROXIMITY',
                    command: 'US_RIGHT',
                    value: distance
                });
            }
        });
    }
}


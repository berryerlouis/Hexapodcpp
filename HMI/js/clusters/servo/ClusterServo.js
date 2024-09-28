import {ClusterName, CommandServo} from "../../protocol/Cluster.js";

export class ClusterServo {
    constructor(messageManager, databaseManager, robot) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.GET_ALL, (message) => {
            if (message.size === 18) {
                for (let i = 0; i < 18; i++) {
                    let angle = message.fetchInt8U();
                    this.robot.moveServo(i, angle);
                    this.databaseManager.updateDb({
                        cluster: 'SERVO',
                        command: i,
                        item: 'angle',
                        value: angle
                    });
                }
            }
        });

        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.GET_ANGLE, (message) => {
            if (message.size === 2) {
                let servoId = message.fetchInt8U();
                let angle = message.fetchInt8U();
                this.robot.moveServo(servoId, angle);
                this.databaseManager.updateDb({
                    cluster: 'SERVO',
                    command: servoId,
                    item: 'angle',
                    value: angle
                });
            }
        });

        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.GET_MIN, (message) => {
            if (message.size === 2) {
                this.databaseManager.updateDb({
                    cluster: 'SERVO',
                    command: message.fetchInt8U(),
                    item: 'min',
                    value: message.fetchInt8U()
                });
            }
        });

        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.GET_MAX, (message) => {
            if (message.size === 2) {
                this.databaseManager.updateDb({
                    cluster: 'SERVO',
                    command: message.fetchInt8U(),
                    item: 'max',
                    value: message.fetchInt8U()
                });
            }
        });

        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.GET_OFFSET, (message) => {
            if (message.size === 2) {
                this.databaseManager.updateDb({
                    cluster: 'SERVO',
                    command: message.fetchInt8U(),
                    item: 'offset',
                    value: message.fetchInt8S()
                });
            }
        });

        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.GET_REVERSE, (message) => {
            if (message.size === 2) {
                this.databaseManager.updateDb({
                    cluster: 'SERVO',
                    command: message.fetchInt8U(),
                    item: 'reverse',
                    value: message.fetchInt8U() !== 0
                });
            }
        });

        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.GET_STATE, (message) => {
            if (message.size === 2) {
                this.databaseManager.updateDb({
                    cluster: 'SERVO',
                    command: message.fetchInt8U(),
                    item: 'state',
                    value: message.fetchInt8U() !== 0
                });
            }
        });
    }
}


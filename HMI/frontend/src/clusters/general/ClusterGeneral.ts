import { ClusterName, CommandGeneral } from "../../protocol/Cluster.js";
import { services } from "../db.js";

export class ClusterGeneral {
    private messageManager: any;
    private databaseManager: any;
    private robot: any;

    constructor(messageManager: any, databaseManager: any, robot: any) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;
        this.initialize();
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.VERSION, (message: any) => {
            if (message.size > 0) {
                let major = message.fetchInt8U();
                let minor = message.fetchInt8U();
                this.databaseManager.updateDb({
                    cluster: 'GENERAL',
                    command: 'VERSION',
                    value: major + '.' + minor
                });
                $('#hexapod-version')[0].innerText = major + '.' + minor;
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.MIN_EXECUTION_TIME, (message: any) => {
            if (message.size > 0) {
                let service = message.fetchInt8U();
                this.databaseManager.updateDb({
                    cluster: 'GENERAL',
                    command: 'MIN_EXECUTION_TIME',
                    item: services[service],
                    value: message.fetchInt16U()
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.MAX_EXECUTION_TIME, (message: any) => {
            if (message.size > 0) {
                let service = message.fetchInt8U();
                this.databaseManager.updateDb({
                    cluster: 'GENERAL',
                    command: 'MAX_EXECUTION_TIME',
                    item: services[service],
                    value: message.fetchInt16U()
                });
            }
        });
    }
}
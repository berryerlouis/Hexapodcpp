import {ClusterName, CommandGeneral} from "../../protocol/Cluster.js";
import {services} from "../db.js";

export class ClusterGeneral {
    constructor(messageManager, databaseManager) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.VERSION, (message) => {
            if (message.size > 0) {
                let major = message.fetchInt8U();
                let minor = message.fetchInt8U();
                this.databaseManager.updateDb({
                    cluster: 'GENERAL',
                    command: 'VERSION',
                    value: major + '.' + minor
                });
                $('#hexapod-version').text(major + '.' + minor);
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.MIN_EXECUTION_TIME, (message) => {
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
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.MAX_EXECUTION_TIME, (message) => {
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


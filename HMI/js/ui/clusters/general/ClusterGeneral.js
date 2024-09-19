import {ClusterName, CommandGeneral} from "../../../protocol/Cluster.js";
import {clusters, services} from "../../../clusters/db.js";

export class ClusterGeneral {
    constructor(messageManager) {
        this.messageManager = messageManager;
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.VERSION, (message) => {
            if (message.size > 0) {
                let major = message.fetchInt8U();
                let minor = message.fetchInt8U();
                clusters['GENERAL'].VERSION = major + '.' + minor;
                $('#hexapod-version').text(major + '.' + minor);
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.MIN_EXECUTION_TIME, (message) => {
            if (message.size > 0) {
                let service = message.fetchInt8U();
                clusters['GENERAL'].MIN_EXECUTION_TIME[services[service]] = message.fetchInt16U();
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.MAX_EXECUTION_TIME, (message) => {
            if (message.size > 0) {
                let service = message.fetchInt8U();
                clusters['GENERAL'].MAX_EXECUTION_TIME[services[service]] = message.fetchInt16U();
            }
        });
    }
}


import {ClusterName, CommandGeneral} from "../../../protocol/Cluster.js";

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
                $('#hexapod-version').text(major + '.' + minor);
            }
        });
    }
}


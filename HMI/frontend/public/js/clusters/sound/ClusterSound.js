import { ClusterName, CommandSound } from "../../protocol/Cluster.js";
export class ClusterSound {
    constructor(messageManager, databaseManager, robot) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;
        this.initialize();
    }
    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SOUND, CommandSound.SOUND_STATUS, (message) => {
            if (message.size === 2) {
                // Add your logic here
            }
        });
    }
}

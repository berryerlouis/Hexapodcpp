import {clustersDatabase} from "../../clusters/db.js";
import {Message} from "../../protocol/Message.js";
import {ClusterName, CommandServo} from "../../protocol/Cluster.js";

export default class ClusterCommand {
    gMessageManager = undefined;

    constructor(guiControls, messageManager) {
        ClusterCommand.gMessageManager = messageManager;
        this.guiControls = guiControls;

        this.commandsFolder = this.guiControls.addFolder(this.guiControls.gui, 'Commands');

        let clusterName = 'GENERAL';
        let clusterFolder = this.guiControls.addFolder(this.commandsFolder, clusterName);
        let cluster = clustersDatabase[clusterName];
        let commandName = 'VERSION';
        clusterFolder.add(cluster, commandName);
        commandName = 'MIN_EXECUTION_TIME';
        let command = clustersDatabase[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'BATTERY');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'CONTROL');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'COMMUNICATION');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'DISPLAY');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'GENERAL');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'ORIENTATION');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'PROXIMITY');
        commandName = 'MAX_EXECUTION_TIME';
        command = clustersDatabase[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'BATTERY');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'CONTROL');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'COMMUNICATION');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'DISPLAY');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'GENERAL');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'ORIENTATION');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'PROXIMITY');

        clusterName = 'IMU';
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, clusterName);
        commandName = 'ACC';
        command = clustersDatabase[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'x')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'y')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'z');
        commandName = 'GYR';
        command = clustersDatabase[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'x')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'y')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'z');
        commandName = 'MAG';
        command = clustersDatabase[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'x')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'y')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'z');
        commandName = 'TMP';
        command = clustersDatabase[clusterName];
        clusterFolder.add(command, commandName);

        clusterName = 'PROXIMITY';
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, clusterName);
        commandName = 'US_LEFT';
        command = clustersDatabase[clusterName];
        clusterFolder.add(command, commandName);
        commandName = 'US_RIGHT';
        command = clustersDatabase[clusterName];
        clusterFolder.add(command, commandName);
        commandName = 'LASER';
        command = clustersDatabase[clusterName];
        clusterFolder.add(command, commandName);
        commandName = 'SET_THRESHOLD';
        command = clustersDatabase[clusterName];
        clusterFolder.add(command, commandName);

        clusterName = 'SERVO';
        command = clustersDatabase[clusterName];
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, clusterName);
        for (let i = 0; i < 18; i++) {
            commandName = i;
            let commandFolder = this.guiControls.addFolder(clusterFolder, commandName);
            commandFolder.add(command[i], 'angle').min(0).max(180).step(1).onFinishChange(
                function () {
                    ClusterCommand.gMessageManager.write(
                        new Message().build(
                            "Tx",
                            ClusterName.SERVO,
                            CommandServo.SET_ANGLE,
                            2,
                            [
                                parseInt(this.domElement.parentElement.parentElement.parentElement.textContent.replace('angleminmaxoffsetreversestate', '')).toString(16),
                                this.getValue().toString(16)]
                        )
                    );
                }
            );
            commandFolder.add(command[i], 'min').min(0).max(180).step(1);
            commandFolder.add(command[i], 'max').min(0).max(180).step(1);
            commandFolder.add(command[i], 'offset').min(-90).max(90).step(1);
            commandFolder.add(command[i], 'reverse');
            commandFolder.add(command[i], 'state');
        }


        //commandFolder = this.guiControls.addFolder(this.commandsFolder, 'BATTERY');
        //commandFolder = this.guiControls.addFolder(this.commandsFolder, 'BODY');

        /*for (let clusterName in clustersDatabase) {
            let commandFolder = this.guiControls.addFolder(this.commandsFolder, clusterName);
            let cluster = clustersDatabase[clusterName];
            for (let key in cluster) {
                if (typeof cluster[key] === 'object' && !Array.isArray(cluster[key])) {
                    let commandValuesFolder = this.guiControls.addFolder(commandFolder, key);
                    for (let subKey in cluster[key]) {
                        if (typeof cluster[key][subKey] === 'object' && !Array.isArray(cluster[key][subKey])) {
                            let commandValuesStructFolder = this.guiControls.addFolder(commandValuesFolder, subKey);
                            for (let substructKey in cluster[key][subKey]) {
                                commandValuesStructFolder.add(cluster[key][subKey], substructKey).min(0).max(100).step(1);
                            }
                        } else {
                            commandValuesFolder.add(cluster[key], subKey);
                        }
                    }
                } else {
                    commandFolder.add(cluster, key);
                }
            }
        }*/
    }
}

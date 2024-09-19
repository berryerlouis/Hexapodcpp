import {clusters} from "../../clusters/db.js";

export default class ClusterCommand {

    constructor(guiControls) {
        this.guiControls = guiControls;

        this.commandsFolder = this.guiControls.addFolder(this.guiControls.gui, 'Commands');

        let clusterName = 'GENERAL';
        let clusterFolder = this.guiControls.addFolder(this.commandsFolder, clusterName);
        let cluster = clusters[clusterName];
        let commandName = 'VERSION';
        clusterFolder.add(cluster, commandName);
        commandName = 'MIN_EXECUTION_TIME';
        let command = clusters[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'BATTERY');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'CONTROL');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'COMMUNICATION');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'DISPLAY');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'GENERAL');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'ORIENTATION');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'PROXIMITY');
        commandName = 'MAX_EXECUTION_TIME';
        command = clusters[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'BATTERY');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'CONTROL');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'COMMUNICATION');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'DISPLAY');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'GENERAL');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'ORIENTATION');
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'PROXIMITY');

        clusterName = 'IMU';
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, clusterName);
        commandName = 'ALL';
        command = clusters[clusterName];
        clusterFolder.add(command, commandName);
        commandName = 'ACC';
        command = clusters[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'x')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'y')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'z');
        commandName = 'GYR';
        command = clusters[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'x')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'y')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'z');
        commandName = 'MAG';
        command = clusters[clusterName][commandName];
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'x')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'y')
        this.guiControls.addFolder(clusterFolder, commandName).add(command, 'z');
        commandName = 'TMP';
        command = clusters[clusterName];
        clusterFolder.add(command, commandName);

        clusterName = 'PROXIMITY';
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, clusterName);
        commandName = 'US_LEFT';
        command = clusters[clusterName];
        clusterFolder.add(command, commandName);
        commandName = 'US_RIGHT';
        command = clusters[clusterName];
        clusterFolder.add(command, commandName);
        commandName = 'LASER';
        command = clusters[clusterName];
        clusterFolder.add(command, commandName);
        commandName = 'SET_THRESHOLD';
        command = clusters[clusterName];
        clusterFolder.add(command, commandName);


        let commandFolder = this.guiControls.addFolder(this.commandsFolder, 'IMU');
        commandFolder = this.guiControls.addFolder(this.commandsFolder, 'PROXIMITY');
        commandFolder = this.guiControls.addFolder(this.commandsFolder, 'SERVO');
        commandFolder = this.guiControls.addFolder(this.commandsFolder, 'BATTERY');
        commandFolder = this.guiControls.addFolder(this.commandsFolder, 'BODY');

        /*for (let clusterName in clusters) {
            let commandFolder = this.guiControls.addFolder(this.commandsFolder, clusterName);
            let cluster = clusters[clusterName];
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

import {clustersDatabase} from "../../clusters/db.js";
import {Message} from "../../protocol/Message.js";
import {
    ClusterName,
    CommandBattery,
    CommandBody,
    CommandGeneral,
    CommandImu,
    CommandProximity,
    CommandServo
} from "../../protocol/Cluster.js";

export default class ClusterCommand {

    constructor(guiControls, messageManager, robot) {
        ClusterCommand.messageManager = messageManager;
        ClusterCommand.robot = robot;

        this.initCallbacks();

        this.guiControls = guiControls;

        let clusterFolder;
        let clusterName;
        let commandName;
        let command;
        this.commandsFolder = this.guiControls.addFolder(this.guiControls.gui, 'Commands');

        //ClusterName.GENERAL
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, ClusterName.GENERAL);
        this.addClusterSimpleCommand(clusterFolder, ClusterName.GENERAL, CommandGeneral.VERSION);
        this.addClusterManyCommands(clusterFolder, ClusterName.GENERAL, CommandGeneral.MIN_EXECUTION_TIME, ['BATTERY', 'CONTROL', 'COMMUNICATION', 'DISPLAY', 'GENERAL', 'ORIENTATION', 'PROXIMITY']);
        this.addClusterManyCommands(clusterFolder, ClusterName.GENERAL, CommandGeneral.MAX_EXECUTION_TIME, ['BATTERY', 'CONTROL', 'COMMUNICATION', 'DISPLAY', 'GENERAL', 'ORIENTATION', 'PROXIMITY']);

        //ClusterName.IMU
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, ClusterName.IMU);
        this.addClusterManyCommands(clusterFolder, ClusterName.IMU, CommandImu.ACC, ['x', 'y', 'z']);
        this.addClusterManyCommands(clusterFolder, ClusterName.IMU, CommandImu.GYR, ['x', 'y', 'z']);
        this.addClusterManyCommands(clusterFolder, ClusterName.IMU, CommandImu.MAG, ['x', 'y', 'z']);
        this.addClusterSimpleCommand(clusterFolder, ClusterName.IMU, CommandImu.TMP);

        //ClusterName.PROXIMITY
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, ClusterName.PROXIMITY);
        this.addClusterSimpleCommand(clusterFolder, ClusterName.PROXIMITY, CommandProximity.US_LEFT);
        this.addClusterSimpleCommand(clusterFolder, ClusterName.PROXIMITY, CommandProximity.US_RIGHT);
        this.addClusterSimpleCommand(clusterFolder, ClusterName.PROXIMITY, CommandProximity.LASER);
        this.addClusterSimpleCommand(clusterFolder, ClusterName.PROXIMITY, CommandProximity.SET_THRESHOLD);

        //ClusterName.BATTERY
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, ClusterName.BATTERY);
        this.addClusterSimpleCommand(clusterFolder, ClusterName.BATTERY, CommandBattery.VOLTAGE);
        this.addClusterSimpleCommand(clusterFolder, ClusterName.BATTERY, CommandBattery.STATUS, {
            Nominal: 0,
            Warning: 1,
            Critical: 2,
            Unknown: 255
        });

        //ClusterName.SERVO
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, ClusterName.SERVO);
        command = clustersDatabase[ClusterName.SERVO];
        for (let i = 0; i < 18; i++) {
            commandName = i;

            let commandFolder = this.guiControls.addFolder(clusterFolder, commandName);
            commandFolder.add(command[i], 'angle').min(0).max(180).step(1).onFinishChange(function (value) {
                sendCommand(ClusterName.SERVO, CommandServo.SET_ANGLE, 2, [getIndexServo(this.domElement), value.toString(16)]);
                //ClusterCommand.robot.moveServo(getIndexServo(this.domElement), value - 90);
            });
            commandFolder.add(command[i], 'min').min(0).max(180).step(1).onFinishChange(function (value) {
                sendCommand(ClusterName.SERVO, CommandServo.SET_MIN, 2, [getIndexServo(this.domElement), value.toString(16)]);
            });
            commandFolder.add(command[i], 'max').min(0).max(180).step(1).onFinishChange(function (value) {
                sendCommand(ClusterName.SERVO, CommandServo.SET_MAX, 2, [getIndexServo(this.domElement), value.toString(16)]);
            });
            commandFolder.add(command[i], 'offset').min(-90).max(90).step(1).onFinishChange(function (value) {
                sendCommand(ClusterName.SERVO, CommandServo.SET_OFFSET, 2, [getIndexServo(this.domElement), (0xFF & value).toString(16)]);
            });
            commandFolder.add(command[i], 'reverse').onFinishChange(function (value) {
                sendCommand(ClusterName.SERVO, CommandServo.SET_REVERSE, 2, [getIndexServo(this.domElement), value ? '01' : '00']);
            });
            commandFolder.add(command[i], 'state').onFinishChange(function (value) {
                sendCommand(ClusterName.SERVO, CommandServo.SET_STATE, 2, [getIndexServo(this.domElement), value ? '01' : '00']);
            });
        }

        //ClusterName.BODY
        clusterFolder = this.guiControls.addFolder(this.commandsFolder, ClusterName.BODY);
        this.addClusterManyCommands(clusterFolder, ClusterName.BODY, 'POSITION',
            ['x', 'y', 'z'],
            function () {
                sendCommandBody();
            },
            -90,
            90,
            1);
        this.addClusterManyCommands(clusterFolder, ClusterName.BODY, 'ROTATION',
            ['x', 'y', 'z'],
            function () {
                sendCommandBody();
            },
            -90,
            90,
            1);
        this.addClusterSimpleCommand(clusterFolder, ClusterName.BODY, 'DELAY',
            undefined,
            function () {
                sendCommandBody();
            },
            1000,
            10000,
            100);


        function getIndexServo(domElement) {
            return parseInt(domElement.parentElement.parentElement.parentElement.textContent.replace('angleminmaxoffsetreversestate', '')).toString(16);
        }

        function sendCommandBody() {
            let command = clustersDatabase['BODY'];
            sendCommand(ClusterName.BODY, CommandBody.SET_X_Y_Z, 14, [
                ...hexStringToArray((command['POSITION'].x & 0xFFFF).toString(16).padStart(4, '0'))
                , ...hexStringToArray((command['POSITION'].y & 0xFFFF).toString(16).padStart(4, '0'))
                , ...hexStringToArray((command['POSITION'].z & 0xFFFF).toString(16).padStart(4, '0'))
                , ...hexStringToArray((command['ROTATION'].x & 0xFFFF).toString(16).padStart(4, '0'))
                , ...hexStringToArray((command['ROTATION'].y & 0xFFFF).toString(16).padStart(4, '0'))
                , ...hexStringToArray((command['ROTATION'].z & 0xFFFF).toString(16).padStart(4, '0'))
                , ...hexStringToArray((command['DELAY'] & 0xFFFF).toString(16).padStart(4, '0'))]);
        }

        function hexStringToArray(hexString) {
            let result = [];
            for (let i = 0; i < hexString.length; i += 2) {
                result.push(hexString.substring(i, i + 2));
            }
            // Inverse l'ordre du tableau
            return result.reverse();
        }

        function sendCommand(cluster, command, size, params) {
            ClusterCommand.messageManager.write(
                new Message().build(
                    "Tx",
                    cluster,
                    command,
                    size,
                    params
                )
            );
        }
    }

    addClusterManyCommands(clusterFolder, clusterName, commandName, subCommands = [], callback, min, max, step) {
        let command = clustersDatabase[clusterName][commandName];
        let commandFolder = this.guiControls.addFolder(clusterFolder, commandName);

        subCommands.forEach(subCommand => {
            let folder = commandFolder.add(command, subCommand);
            if (min && max && step) folder = folder.min(min).max(max).step(step);
            if (callback) folder.onFinishChange(callback);
        });
    }

    addClusterSimpleCommand(clusterFolder, clusterName, commandName, subCommands, callback, min, max, step) {
        const command = clustersDatabase[clusterName];
        const addCommand = (cmd, name, subs) => {
            let folder = clusterFolder.add(cmd, name, subs);
            if (min && max && step) folder = folder.min(min).max(max).step(step);
            if (callback) folder.onFinishChange(callback);
        };

        if (subCommands) {
            addCommand(command, commandName, subCommands);
        } else {
            addCommand(command, commandName);
        }
    }

    initCallbacks() {

        ClusterCommand.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.GET_MIN, (message) => {
            if (message.size === 2) {
                let servoId = message.fetchInt8U();
                this.guiControls.gui.__folders["Commands"].__folders['SERVO'].__folders[servoId].__controllers[0].__min = message.fetchInt8U();
            }
        });
        ClusterCommand.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.SET_MIN, (message) => {
            if (message.size === 2) {
                let servoId = message.fetchInt8U();
                this.guiControls.gui.__folders["Commands"].__folders['SERVO'].__folders[servoId].__controllers[0].__min = message.fetchInt8U();
            }
        });
        ClusterCommand.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.SET_MAX, (message) => {
            if (message.size === 2) {
                let servoId = message.fetchInt8U();
                this.guiControls.gui.__folders["Commands"].__folders['SERVO'].__folders[servoId].__controllers[0].__max = message.fetchInt8U();
            }
        });
        ClusterCommand.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SERVO, CommandServo.GET_MAX, (message) => {
            if (message.size === 2) {
                let servoId = message.fetchInt8U();
                this.guiControls.gui.__folders["Commands"].__folders['SERVO'].__folders[servoId].__controllers[0].__max = message.fetchInt8U();
            }
        });
    }
}


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
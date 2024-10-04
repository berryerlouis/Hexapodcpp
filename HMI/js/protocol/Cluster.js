import {ClusterNotFoundError, CommandNotFoundError} from './ClusterError.js';

class ClusterName {
    static GENERAL = 'GENERAL';
    static IMU = 'IMU';
    static PROXIMITY = 'PROXIMITY';
    static SERVO = 'SERVO';
    static BATTERY = 'BATTERY';
    static BODY = 'BODY';
    static GENERIC = 'GENERIC';
}


class Cluster {
    constructor(name, code, commands) {
        this.name = name;
        this.code = code;
        this.commands = [...commands, new Command(CommandNack.NACK, CodeCommandNack)];
    }
}

class Command {
    code;
    name;

    constructor(name, code) {
        this.name = name;
        this.code = code;
    }
}

const CodeCommandNack = 'FF';

class CommandNack {
    static NACK = 'NACK';
}

const CommandGeneral = {
    VERSION: 'VERSION',
    MIN_EXECUTION_TIME: 'MIN_EXECUTION_TIME',
    MAX_EXECUTION_TIME: 'MAX_EXECUTION_TIME',
    INSTANT_EXECUTION_TIME: 'INSTANT_EXECUTION_TIME',
    RESET_EXECUTION_TIME: 'RESET_EXECUTION_TIME',
};

const CommandImu = {
    ALL: 'ALL',
    ACC: 'ACC',
    GYR: 'GYR',
    MAG: 'MAG',
    TMP: 'TMP',
    YAWPITCHROLL: 'YAWPITCHROLL',
};

const CommandBody = {
    SET_X_Y_Z: 'SET_X_Y_Z'
};

const CommandProximity = {
    US_LEFT: 'US_LEFT',
    US_RIGHT: 'US_RIGHT',
    LASER: 'LASER',
    SET_THRESHOLD: 'SET_THRESHOLD'
};

const CommandBattery = {
    VOLTAGE: 'VOLTAGE',
    STATUS: 'STATUS'
};

const CommandServo = {
    GET_ALL: 'GET_ALL',
    GET_ANGLE: 'GET_ANGLE',
    SET_ANGLE: 'SET_ANGLE',
    GET_MIN: 'GET_MIN',
    SET_MIN: 'SET_MIN',
    GET_MAX: 'GET_MAX',
    SET_MAX: 'SET_MAX',
    GET_OFFSET: 'GET_OFFSET',
    SET_OFFSET: 'SET_OFFSET',
    GET_STATE: 'GET_STATE',
    SET_STATE: 'SET_STATE',
    GET_REVERSE: 'GET_REVERSE',
    SET_REVERSE: 'SET_REVERSE',
    SAVE: 'SAVE'
};


export default class Clusters {
    static clusters = [
        new Cluster(
            ClusterName.GENERAL, '00',
            [
                new Command(CommandGeneral.VERSION, '00'),
                new Command(CommandGeneral.MIN_EXECUTION_TIME, '01'),
                new Command(CommandGeneral.MAX_EXECUTION_TIME, '02'),
                new Command(CommandGeneral.INSTANT_EXECUTION_TIME, '03'),
                new Command(CommandGeneral.RESET_EXECUTION_TIME, '04')
            ]
        ),
        new Cluster(
            ClusterName.IMU, '01',
            [
                new Command(CommandImu.ALL, '00'),
                new Command(CommandImu.ACC, '01'),
                new Command(CommandImu.GYR, '02'),
                new Command(CommandImu.MAG, '03'),
                new Command(CommandImu.TMP, '04'),
                new Command(CommandImu.YAWPITCHROLL, '05')
            ]
        ),
        new Cluster(
            ClusterName.PROXIMITY, '02',
            [
                new Command(CommandProximity.US_LEFT, '00'),
                new Command(CommandProximity.US_RIGHT, '01'),
                new Command(CommandProximity.LASER, '02'),
                new Command(CommandProximity.SET_THRESHOLD, '03')
            ]
        ),
        new Cluster(
            ClusterName.SERVO, '03',
            [
                new Command(CommandServo.GET_ALL, '00'),
                new Command(CommandServo.GET_ANGLE, '01'),
                new Command(CommandServo.SET_ANGLE, '02'),
                new Command(CommandServo.GET_MIN, '03'),
                new Command(CommandServo.SET_MIN, '04'),
                new Command(CommandServo.GET_MAX, '05'),
                new Command(CommandServo.SET_MAX, '06'),
                new Command(CommandServo.GET_OFFSET, '07'),
                new Command(CommandServo.SET_OFFSET, '08'),
                new Command(CommandServo.GET_STATE, '09'),
                new Command(CommandServo.SET_STATE, '0A'),
                new Command(CommandServo.GET_REVERSE, '0B'),
                new Command(CommandServo.SET_REVERSE, '0C'),
                new Command(CommandServo.SAVE, '0D')
            ]
        ),
        new Cluster(
            ClusterName.BATTERY, '04',
            [
                new Command(CommandBattery.VOLTAGE, '00'),
                new Command(CommandBattery.STATUS, '01')
            ]
        ),
        new Cluster(
            ClusterName.BODY, '05',
            [
                new Command(CommandBody.SET_X_Y_Z, '00')
            ]
        )
    ];


    static getClusterByNameOrCode(identifier, isCode = false) {
        const clusterFound = Clusters.clusters.find(cluster =>
            isCode ? cluster.code.toLowerCase() === identifier.toLowerCase() : cluster.name.toLowerCase() === identifier.toLowerCase()
        );
        if (!clusterFound) {
            throw new ClusterNotFoundError(`Cluster with ${isCode ? 'code' : 'name'} "${identifier}" not found`);
        }
        return clusterFound;
    }

    static getClusterByName(clusterName) {
        const cluster = Clusters.getClusterByNameOrCode(clusterName);
        return {
            code: cluster.code,
            name: cluster.name
        }
    }

    static getClusterByCode(clusterCode) {
        const cluster = Clusters.getClusterByNameOrCode(clusterCode, true);
        return {
            code: cluster.code,
            name: cluster.name
        }
    }

    static getCommandByName(cluster, commandName) {
        const commands = Clusters.getClusterByNameOrCode(cluster.name).commands;
        const commandFound = commands.find(cmd => cmd.name.toLowerCase() === commandName.toLowerCase());
        if (!commandFound) {
            throw new CommandNotFoundError(`Command with name "${commandName}" not found`);
        }
        return commandFound;
    }

    static getCommandByCode(cluster, commandCode) {
        const commands = Clusters.getClusterByNameOrCode(cluster.name).commands;
        const commandFound = commands.find(cmd => cmd.code.toLowerCase() === commandCode.toLowerCase());
        if (!commandFound) {
            throw new CommandNotFoundError(`Command with code "${commandCode}" not found`);
        }
        return commandFound;
    }


}


export {
    Clusters,
    Cluster,
    Command,
    ClusterName,
    CodeCommandNack,
    CommandGeneral,
    CommandImu,
    CommandProximity,
    CommandBattery,
    CommandBody,
    CommandServo
};

class ClusterName {
    static GENERAL = 'GENERAL';
    static IMU = 'IMU';
    static PROXIMITY = 'PROXIMITY';
    static SERVO = 'SERVO';
    static BATTERY = 'BATTERY';
    static BODY = 'BODY';
}


class Cluster {
    code;
    name;
    commands = [];
    constructor(name, code, commands) {
        this.name = name;
        this.code = code;
        this.commands = commands;
        this.commands.push(new ClusterCommandGeneric());
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

class ClusterCommandGeneric extends Command {
    constructor() {
        super(CommandGeneric.GENERIC, 'FF');
    }
}

class CommandGeneric {
    static GENERIC = 'GENERIC';
}
class CommandGeneral {
    static VERSION = 'VERSION';
    static MIN_EXECUTION_TIME = 'MIN_EXECUTION_TIME';
    static MAX_EXECUTION_TIME = 'MAX_EXECUTION_TIME';
    static INSTANT_EXECUTION_TIME = 'INSTANT_EXECUTION_TIME';
    static RESET_EXECUTION_TIME = 'RESET_EXECUTION_TIME';
}
class CommandImu {
    static ALL = 'ALL';
    static ACC = 'ACC';
    static GYR = 'GYR';
    static MAG = 'MAG';
    static TMP = 'TMP';
}
class CommandBody {
    static SET_X_Y_Z = 'SET_X_Y_Z';
}
class CommandProximity {
    static US_LEFT = 'US_LEFT';
    static US_RIGHT = 'US_RIGHT';
    static LAZER = 'LAZER';
}
class CommandBattery {
    static VOLTAGE = 'VOLTAGE';
    static STATUS = 'STATUS';
}
class CommandServo {
    static GET_ALL = 'GET_ALL';
    static GET_ANGLE = 'GET_ANGLE';
    static SET_ANGLE = 'SET_ANGLE';
    static GET_MIN = 'GET_MIN';
    static SET_MIN = 'SET_MIN';
    static GET_MAX = 'GET_MAX';
    static SET_MAX = 'SET_MAX';
    static GET_OFFSET = 'GET_OFFSET';
    static SET_OFFSET = 'SET_OFFSET';
    static GET_STATE = 'GET_STATE';
    static SET_STATE = 'SET_STATE';
    static GET_REVERSE = 'GET_REVERSE';
    static SET_REVERSE = 'SET_REVERSE';
    static SAVE = 'SAVE';
}


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
                new Command(CommandImu.TMP, '04')
            ]
        ),
        new Cluster(
            ClusterName.PROXIMITY, '02',
            [
                new Command(CommandProximity.US_LEFT, '00'),
                new Command(CommandProximity.US_RIGHT, '01'),
                new Command(CommandProximity.LAZER, '02'),
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
                new Command(CommandServo.SAVE, '0D'),
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

    static findClusterByName(name) {
        let clusterFound = false;
        Clusters.clusters.forEach(cluster => {
            if (cluster.name == name) {
                clusterFound = cluster;
            }
        });
        if (!clusterFound) {
            throw 'error cluster not found'
        }
        return clusterFound;
    }

    static findCommandByName(command) {
        let commandFound = false;
        Clusters.clusters.forEach(cluster => {
            cluster.commands.forEach(cmd => {
                if (command === cmd.name) {
                    commandFound = cmd;
                }
            });
        });
        if (!commandFound) {
            throw 'error command not found'
        }
        return commandFound;
    }

    static findClusterByCode(code) {
        const clusterFound = Clusters.clusters.find(cluster => cluster.code === code);
        return clusterFound;
    }

    static findCommandByCommandName(command) {
        let commandFound = false;
        Clusters.clusters.forEach(cluster => {
            cluster.commands.forEach(cmd => {
                if (command === cmd.name) {
                    commandFound = cmd;
                }
            });
        });
        if (!commandFound) {
            throw 'error command not found'
        }
        return commandFound;
    }

    static findCommandByCode(cluster, code) {
        const commandFound = cluster.commands.find(command => command.code.toUpperCase() === code.toUpperCase());
        return commandFound;
    }
}


export { Clusters, Cluster, ClusterName, CommandGeneral, CommandImu, CommandProximity, CommandBattery, CommandBody, CommandServo };
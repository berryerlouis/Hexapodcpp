import {Cluster, ClusterName, Command, CommandName} from "./clusterType.ts";
import ClusterGeneral from "./clusterGeneral.ts";
import ClusterImu from "./clusterImu.ts";
import ClusterProximity from "./clusterProximity.ts";
import ClusterServo from "./clusterServo.ts";
import ClusterBattery from "./clusterBattery.ts";
import ClusterBody from "./clusterBody.ts";
import ClusterButton from "./clusterButton.ts";
import ClusterSound from "./clusterSound.ts";

export class ClusterNotFoundError extends Error {
    constructor(message: string) {
        super(message);
        this.name = 'ClusterNotFoundError';
    }
}

export class CommandNotFoundError extends Error {
    constructor(message: string) {
        super(message);
        this.name = 'CommandNotFoundError';
    }
}

const clusters: Cluster[] = [
    new ClusterGeneral(),
    new ClusterImu(),
    new ClusterProximity(),
    new ClusterServo(),
    new ClusterBattery(),
    new ClusterBody(),
    new ClusterButton(),
    new ClusterSound(),
];

export function getClusterByCode(code: string): Cluster {
    const cluster = clusters.find(cluster => cluster.code.toLowerCase() === code.toLowerCase());
    if (!cluster) {
        throw new ClusterNotFoundError(`Cluster with code "${code}" not found`);
    }
    return cluster;
}

export function getClusterByName(name: ClusterName): Cluster {
    const cluster = clusters.find(cluster => cluster.name.toLowerCase() === name.toLowerCase());
    if (!cluster) {
        throw new ClusterNotFoundError(`Cluster with name "${name}" not found`);
    }
    return cluster;
}

export function getCommandByCode(cluster: Cluster, code: string): Command {
    const command = cluster.commands.find(command => command.code.toLowerCase() === code.toLowerCase());
    if (!command) {
        throw new CommandNotFoundError(`Command with code "${command}" not found`);
    }
    return command;
}

export function getCommandByName(cluster: Cluster, name: CommandName): Command {
    const command = cluster.commands.find(command => command.name.toLowerCase() === name.toLowerCase());
    if (!command) {
        throw new CommandNotFoundError(`Command with name "${command}" not found`);
    }
    return command;
}

export function getErrorName(errorCode: number): string {
    switch (errorCode) {
        case 0xFF:
            return 'Error';
        case 0xFF - 1:
            return 'Error argument';
        case 0xFF - 2:
            return 'Error null pointer';
        case 0xFF - 3:
            return 'Error overload';
        case 0xFF - 4:
            return 'Error size';
        case 0xFF - 5:
            return 'Error min';
        case 0xFF - 6:
            return 'Error max';
        case 0xFF - 7:
            return 'Error disable';
        case 0xFF - 8:
            return 'Error unknown cluster';
        default:
            return 'Unknown error';
    }
}

export default clusters;
import {ClusterBatteryCommands} from "./clusterBattery.ts";
import {ClusterButtonCommands} from "./clusterButton.ts";
import {ClusterImuCommands} from "./clusterImu.ts";
import {ClusterGeneralCommands} from "./clusterGeneral.ts";
import {ClusterServoCommands} from "./clusterServo.ts";
import {ClusterBodyCommands} from "./clusterBody.ts";
import {ClusterProximityCommands} from "./clusterProximity.ts";
import {ClusterSoundCommands} from "./clusterSound.ts";

export enum ClusterName {
    GENERAL = 'GENERAL',
    IMU = 'IMU',
    PROXIMITY = 'PROXIMITY',
    SERVO = 'SERVO',
    BATTERY = 'BATTERY',
    BODY = 'BODY',
    BUTTON = 'BUTTON',
    SOUND = 'SOUND',
    GENERIC = 'GENERIC'
}

export enum ClusterGenericCommands {
    GENERIC = 'GENERIC'
}

export type CommandName =
    ClusterGenericCommands
    | ClusterBatteryCommands
    | ClusterBodyCommands
    | ClusterButtonCommands
    | ClusterImuCommands
    | ClusterGeneralCommands
    | ClusterServoCommands
    | ClusterProximityCommands
    | ClusterSoundCommands;

export interface Command {
    code: string;
    name: CommandName;
}

export class Cluster {
    name: ClusterName;
    code: string;
    commands: Command[];

    constructor(name: ClusterName, code: string, commands: Command[]) {
        this.name = name;
        this.code = code;
        this.commands = commands;
    }
}
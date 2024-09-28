import {ClusterName, CommandBattery} from "../../protocol/Cluster.js";

export class ClusterBattery {
    constructor(messageManager, databaseManager, robot) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;

        this.batteryStatusHMI = $('#hexapod-battery-status');
        this.batteryVoltageHMI = $('#hexapod-battery-voltage');
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.BATTERY, CommandBattery.STATUS, (message) => {
            if (message.size > 0) {
                let state = message.fetchInt8U();
                let voltage = message.fetchInt16U() / 100;
                this.databaseManager.updateDb({
                    cluster: 'BATTERY',
                    command: 'STATUS',
                    value: state
                });
                this.databaseManager.updateDb({
                    cluster: 'BATTERY',
                    command: 'VOLTAGE',
                    value: voltage
                });
                if (state === 0) {
                    this.batteryStatusHMI.removeClass('bi-battery');
                    this.batteryStatusHMI.removeClass('bi-battery-half');
                    this.batteryStatusHMI.addClass('bi-battery-full');
                    this.batteryStatusHMI.attr('style', "color: rgb(50, 223, 27);");
                } else if (state === 1) {
                    this.batteryStatusHMI.removeClass('bi-battery');
                    this.batteryStatusHMI.removeClass('bi-battery-full');
                    this.batteryStatusHMI.addClass('bi-battery-half');
                    this.batteryStatusHMI.attr('style', "color: rgb(223, 135, 27);");
                } else if (state === 2) {
                    this.batteryStatusHMI.removeClass('bi-battery-full');
                    this.batteryStatusHMI.removeClass('bi-battery-half');
                    this.batteryStatusHMI.addClass('bi-battery');
                    this.batteryStatusHMI.attr('style', "color: rgb(223, 27, 27);");
                } else {
                    this.batteryStatusHMI.removeClass('bi-battery-full');
                    this.batteryStatusHMI.removeClass('bi-battery-half');
                    this.batteryStatusHMI.addClass('bi-battery');
                    this.batteryStatusHMI.attr('style', "color: rgb(223, 27, 27);");
                }
                this.batteryVoltageHMI[0].innerText = voltage.toFixed(2) + 'V';
            }
        });

        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.BATTERY, CommandBattery.VOLTAGE, (message) => {
            if (message.size > 0) {
                let voltage = message.fetchInt16U() / 100;
                this.databaseManager.updateDb({
                    cluster: 'BATTERY',
                    command: 'VOLTAGE',
                    value: voltage
                });
                this.batteryVoltageHMI[0].innerText = voltage.toFixed(2) + 'V';
            }
        });
    }
}


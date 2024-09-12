import {ClusterName, CommandBattery} from "../../../protocol/Cluster.js";

export class ClusterBattery {
    constructor(messageManager) {
        this.messageManager = messageManager;
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.BATTERY, CommandBattery.STATUS, (message) => {
            if (message.size > 0) {
                let state = message.fetchInt8U();
                let voltage = message.fetchInt16U() / 100;
                if (state === 0) {
                    $('#hexapod-battery-status').removeClass('bi-battery');
                    $('#hexapod-battery-status').removeClass('bi-battery-half');
                    $('#hexapod-battery-status').addClass('bi-battery-full');
                    $('#hexapod-battery-status').attr('style', "color: rgb(50, 223, 27);");
                } else if (state === 1) {
                    $('#hexapod-battery-status').removeClass('bi-battery');
                    $('#hexapod-battery-status').removeClass('bi-battery-full');
                    $('#hexapod-battery-status').addClass('bi-battery-half');
                    $('#hexapod-battery-status').attr('style', "color: rgb(223, 135, 27);");
                } else if (state === 2) {
                    $('#hexapod-battery-status').removeClass('bi-battery-full');
                    $('#hexapod-battery-status').removeClass('bi-battery-half');
                    $('#hexapod-battery-status').addClass('bi-battery');
                    $('#hexapod-battery-status').attr('style', "color: rgb(223, 27, 27);");
                } else {
                    $('#hexapod-battery-status').removeClass('bi-battery-full');
                    $('#hexapod-battery-status').removeClass('bi-battery-half');
                    $('#hexapod-battery-status').addClass('bi-battery');
                    $('#hexapod-battery-status').attr('style', "color: rgb(223, 27, 27);");
                }
                $('#hexapod-battery-voltage').text(voltage.toFixed(2) + 'V');
            }
        });
    }
}


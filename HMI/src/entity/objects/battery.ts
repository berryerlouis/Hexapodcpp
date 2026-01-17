import Message from "../../communication/message.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterBatteryCommands} from "../../communication/clusters/clusterBattery.ts";
import Socket from "../../communication/socket.ts";

interface BatteryStruct {
    status: number;
    voltage: number;
    current: number;
}


export default class Battery {
    interval: number;
    socket: Socket;
    batteryData: BatteryStruct = {status: 0, voltage: 0, current: 0};

    constructor(socket: Socket, intervalCommand: number) {
        this.socket = socket;
        this.interval = 0;
        socket.addSpecificCallbackRead(ClusterName.BATTERY, ClusterBatteryCommands.VOLTAGE, (message: Message) => {
            this.batteryData.voltage = (message.getValueUint16(0)) / 100;
            document.getElementById('voltage')!.innerText = (this.batteryData.voltage).toFixed(2);
        });
        socket.addSpecificCallbackRead(ClusterName.BATTERY, ClusterBatteryCommands.CURRENT, (message: Message) => {
            this.batteryData.current = (message.getValueUint16(0)) / 1000;
            document.getElementById('current')!.innerText = (this.batteryData.current).toFixed(3);
        });
        socket.addSpecificCallbackRead(ClusterName.BATTERY, ClusterBatteryCommands.STATUS, (message: Message) => {
            this.batteryData.status = message.getValueUint8(0);
        });

        this.socket.addCallbackStopped(() => {
            clearInterval(this.interval);
        });

        this.socket.addCallbackStarted(() => {
            this.interval = setInterval(() => {
                this.socket.write(new Message(ClusterName.BATTERY, ClusterBatteryCommands.STATUS));
                this.socket.write(new Message(ClusterName.BATTERY, ClusterBatteryCommands.VOLTAGE));
                this.socket.write(new Message(ClusterName.BATTERY, ClusterBatteryCommands.CURRENT));
            }, intervalCommand);
        });
    }
}
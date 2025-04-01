import Message from "../../communication/message.ts";
import {Direction} from "../../communication/protocol.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterBatteryCommands} from "../../communication/clusters/clusterBattery.ts";
import Socket from "../../communication/socket.ts";

interface BatteryStruct {
    status: number;
    voltage: number;
    current: number;
}


export default class Battery {

    socket:Socket;
    batteryData: BatteryStruct = {status: 0, voltage: 0, current: 0};
    constructor(socket: Socket) {
        this.socket = socket;

        socket.addSpecificCallbackRead(ClusterName.BATTERY, ClusterBatteryCommands.VOLTAGE, (message:Message) => {
            this.batteryData.voltage = (message.params[0] + (message.params[1] << 8))/100;
            document.getElementById('voltage')!.innerText = (this.batteryData.voltage).toFixed(2);
        });
        socket.addSpecificCallbackRead(ClusterName.BATTERY, ClusterBatteryCommands.CURRENT, (message:Message) => {
            this.batteryData.current = (message.params[0] + (message.params[1] << 8))/1000;
            document.getElementById('current')!.innerText = (this.batteryData.current).toFixed(3);
        });
        socket.addSpecificCallbackRead(ClusterName.BATTERY, ClusterBatteryCommands.STATUS, (message:Message) => {
            this.batteryData.status = message.params[0];
        });

        setInterval(()=>{
            this.socket.write(new Message(Direction.TX, ClusterName.BATTERY, ClusterBatteryCommands.STATUS));
            this.socket.write(new Message(Direction.TX, ClusterName.BATTERY, ClusterBatteryCommands.VOLTAGE));
            this.socket.write(new Message(Direction.TX, ClusterName.BATTERY, ClusterBatteryCommands.CURRENT));
        },1000);
    }
}
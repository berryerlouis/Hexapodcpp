import {ClusterName} from "../../communication/clusters/clusterType.ts";
import {ClusterGeneralCommands} from "../../communication/clusters/clusterGeneral.ts";
import Message from "../../communication/message.ts";
import Socket from "../../communication/socket.ts";

export default class LoopTime {
    servicesTimeMax:number;
    servicesTimeMin:number;
    socket:Socket;
    interval: number;
    constructor(socket:Socket, intervalCommand:number) {
        this.socket = socket;
        this.servicesTimeMax = 0;
        this.servicesTimeMin = 0;
        this.interval = intervalCommand;

        this.socket.addSpecificCallbackRead(ClusterName.GENERAL, ClusterGeneralCommands.MAX_EXECUTION_TIME, (message:Message) => {
            if(message.getValueUint8(0) == 4) {
                this.servicesTimeMax = message.getValueUint16(1);
                document.getElementById('max-time')!.innerText = this.servicesTimeMax.toString();
            }
        });

        this.socket.addSpecificCallbackRead(ClusterName.GENERAL, ClusterGeneralCommands.MIN_EXECUTION_TIME, (message:Message) => {
            if(message.getValueUint8(0) == 4) {
                this.servicesTimeMin = message.getValueUint16(1);
                document.getElementById('min-time')!.innerText = this.servicesTimeMin.toString();
            }
        });

        this.socket.addCallbackStopped(()=> {
            clearInterval(this.interval);
        });

        this.socket.addCallbackStarted(()=>{
            this.socket.write(new Message( ClusterName.GENERAL, ClusterGeneralCommands.RESET_TIME));
            this.interval = setInterval(()=>{
                this.socket.write(new Message( ClusterName.GENERAL, ClusterGeneralCommands.MAX_EXECUTION_TIME));
                this.socket.write(new Message( ClusterName.GENERAL, ClusterGeneralCommands.MIN_EXECUTION_TIME));
            }, this.interval);
        });
    }
}
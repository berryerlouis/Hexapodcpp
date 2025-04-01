import Message from "./message.ts";
import Protocol, {Direction} from "./protocol.ts";
import {openPopupError, openPopupInfo, openPopupWarning} from "../engine/popup.ts";
import {ClusterGenericCommands, ClusterName, CommandName} from "./clusters/clusterType.ts";
import {getErrorName} from "./clusters/clusters.ts";


type Callback = (message: any) => void;
type CallbackStarted = () => void;

interface SpecificCallback {
    callback: Callback;
    cluster: ClusterName;
    command: CommandName;
    params?: number[];
}

export default class Socket {

    private listOfSpecificCallbackRead: SpecificCallback[];
    private listOfCallbackRead: Callback[];
    private listOfCallbackWrite: Callback[];
    private readonly callbackStarted: CallbackStarted;
    private socket: WebSocket;
    private messagesList: Message[];

    constructor(callbackStarted:CallbackStarted) {
        this.listOfCallbackRead = [];
        this.listOfCallbackWrite = [];
        this.listOfSpecificCallbackRead = [];
        this.messagesList = []
        this.callbackStarted = callbackStarted;
        this.socket = new WebSocket('ws://192.168.1.170:8080');

        this.socket.addEventListener('open', () => {
            console.log('Connected to the WebSocket server');
            openPopupInfo('Connected!');
            this.callbackStarted && this.callbackStarted();
        });

        this.socket.addEventListener('message', (event) => {
            let frame = Protocol.decode(Direction.RX, event.data);
            frame.setDate();
            this.notifyRead(frame);

            if(this.messagesList.length > 0) {
                if ( frame.command?.name == ClusterGenericCommands.GENERIC ||
                     frame.cluster?.name == ClusterName.GENERIC ||
                     frame.cluster?.code == this.messagesList[0].cluster?.code)  {

                    if(frame.command?.name == ClusterGenericCommands.GENERIC ||
                        frame.cluster?.name == ClusterName.GENERIC) {
                        openPopupWarning(getErrorName(frame.params[0]) + ': ' + this.messagesList[0].raw);
                    }
                    this.messagesList.shift();
                    if (this.messagesList.length > 0) {
                        this.writeOnSocket(this.messagesList[0]).then();
                    }
                }
            }
        });

        this.socket.addEventListener('close', () => {
            console.log('Disconnected from the WebSocket server');
            openPopupWarning('WebSocket disconnected!');
        });

        this.socket.addEventListener('error', (event) => {
            console.error('WebSocket error:', event);
            openPopupError('WebSocket error!');
        });
    }

    isConnected(): boolean {
        return this.socket.readyState === WebSocket.OPEN;
    }

    addCallbackRead(cb: Callback) {
        this.listOfCallbackRead.push(cb);
    }

    addSpecificCallbackRead(cluster:ClusterName, command:CommandName , cb: Callback, params?: number[]) {
        if(params == null) {
            this.listOfSpecificCallbackRead.push({callback:cb, cluster:cluster, command:command});
        } else {
            this.listOfSpecificCallbackRead.push({callback:cb, cluster:cluster, command:command, params:params});
        }
    }

    addCallbackWrite(cb: Callback) {
        this.listOfCallbackWrite.push(cb);
    }

    notifyRead(message: any) {
        console.log(`Received: ${message}`);
        this.listOfCallbackRead.forEach((cb) => {
            cb(message);
        });

        this.listOfSpecificCallbackRead.forEach((speCb) => {
            if(message.cluster?.name == speCb.cluster) {
                if (message.command?.name == speCb.command) {
                    if (message.params && speCb.params) {
                        for (let i = 0; i < speCb.params.length; i++) {
                            if(message.params[i] != speCb.params[i]) {
                                return;
                            }
                        }
                        speCb.callback(message);
                    } else {
                        speCb.callback(message);
                    }
                }
            }
        });
    }

    notifyWrite(message: any) {
        message.setDate();
        console.log(`Transmit: ${message}`);
        this.listOfCallbackWrite.forEach((cb) => {
            cb(message);
        });
    }

    writeOnSocket(message: Message): Promise<boolean> {
        return new Promise((resolve, reject) => {
            if (this.socket.readyState === WebSocket.OPEN) {
                if (message.raw) {
                    this.socket.send(message.raw);
                    this.notifyWrite(message);
                    resolve(true);
                } else {
                    openPopupError('WebSocket write error: message.raw is undefined!');
                    reject(false);
                }
            } else {
                openPopupError('WebSocket write error: WebSocket is not open!');
                reject(false);
            }
        });
    }

    write(message: Message)  {
        this.messagesList.push(message);
        if (this.messagesList.length !== 1) {
            return;
        }
        this.writeOnSocket(message).then();
    }
}
import Message from "./message.ts";
import Protocol, {Direction} from "./protocol.ts";
import {openPopupError, openPopupInfo, openPopupWarning} from "../engine/popup.ts";
import {ClusterGenericCommands, ClusterName, CommandName} from "./clusters/clusterType.ts";
import {getErrorName} from "./clusters/clusters.ts";


type Callback = (message: any) => void;
type CallbackStarted = () => void;
type CallbackStopped = () => void;

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
    private listOfCallbackStarted: CallbackStarted[];
    private listOfCallbackStopped: CallbackStopped[];
    private socket: WebSocket;
    private messagesList: Message[];

    constructor(url: string | URL) {
        this.listOfCallbackRead = [];
        this.listOfCallbackWrite = [];
        this.listOfSpecificCallbackRead = [];
        this.listOfCallbackStarted = [];
        this.listOfCallbackStopped = [];
        this.messagesList = []
        this.socket = new WebSocket(url);

        this.socket.addEventListener('open', () => {
            console.log('Connected to the WebSocket server');
            openPopupInfo('Connected!');
            this.notifyCallbackStarted();
        });

        this.socket.addEventListener('message', (event) => {
            let frame = Protocol.decode(event.data);
            frame.direction = Direction.RX;
            frame.setDate();
            this.notifyRead(frame);

            if (this.messagesList.length > 0) {
                if (frame.command?.name == ClusterGenericCommands.GENERIC ||
                    frame.cluster?.name == ClusterName.GENERIC ||
                    frame.cluster?.code == this.messagesList[0].cluster?.code) {

                    if (frame.command?.name == ClusterGenericCommands.GENERIC ||
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
            this.notifyCallbackStopped();
            this.messagesList = [];
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

    addCallbackStarted(cb: CallbackStarted) {
        this.listOfCallbackStarted.push(cb);
    }

    notifyCallbackStarted() {
        this.listOfCallbackStarted.forEach((speCb) => {
            speCb();
        });
    }

    addCallbackStopped(cb: CallbackStarted) {
        this.listOfCallbackStopped.push(cb);
    }

    notifyCallbackStopped() {
        this.listOfCallbackStopped.forEach((speCb) => {
            speCb();
        });
    }

    addSpecificCallbackRead(cluster: ClusterName, command: CommandName, cb: Callback, params?: number[]) {
        if (params == null) {
            this.listOfSpecificCallbackRead.push({callback: cb, cluster: cluster, command: command});
        } else {
            this.listOfSpecificCallbackRead.push({callback: cb, cluster: cluster, command: command, params: params});
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
            if (message.cluster?.name == speCb.cluster) {
                if (message.command?.name == speCb.command) {
                    if (message.params && speCb.params) {
                        for (let i = 0; i < speCb.params.length; i++) {
                            if (message.params[i] != speCb.params[i]) {
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

    private writeOnSocket(message: Message): Promise<boolean> {
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

    write(message: Message) {
        this.messagesList.push(message);
        let pb: HTMLElement = document.getElementById('progress-message-queue')!;
        if (pb) {
            pb.setAttribute('style', 'width: ' + (this.messagesList.length > 100 ? 100 : this.messagesList.length) + '%');
        }
        if (this.messagesList.length >= 100) {
            openPopupWarning('WebSocket Messages list full!');
        }
        if (this.messagesList.length !== 1) {
            return;
        }
        this.writeOnSocket(message).then();
    }
}
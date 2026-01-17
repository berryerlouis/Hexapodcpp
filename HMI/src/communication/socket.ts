import Message from "./message.ts";
import Protocol, { Direction } from "./protocol.ts";
import { clearAllPopups, openPopupError, openPopupInfo, openPopupWarning } from "../engine/popup.ts";
import { ClusterGenericCommands, ClusterName, CommandName } from "./clusters/clusterType.ts";
import { getErrorName } from "./clusters/clusters.ts";


type Callback = (message: Message) => void;
type CallbackStarted = () => void;
type CallbackStopped = () => void;

interface SpecificCallback {
    callback: Callback;
    cluster: ClusterName;
    command: CommandName;
    params?: number[];
}

const MAX_MESSAGE_QUEUE_SIZE = 100;
const RECONNECT_INTERVAL_MS = 2500;

export default class Socket {

    private listOfSpecificCallbackRead: SpecificCallback[];
    private listOfCallbackRead: Callback[];
    private listOfCallbackWrite: Callback[];
    private listOfCallbackStarted: CallbackStarted[];
    private listOfCallbackStopped: CallbackStopped[];
    private socket: WebSocket;
    private messagesList: Message[];
    private url: string | URL;
    private reconnectTimer: number | null = null;
    private shouldReconnect: boolean = true;

    constructor(url: string | URL) {
        this.url = url;
        this.listOfCallbackRead = [];
        this.listOfCallbackWrite = [];
        this.listOfSpecificCallbackRead = [];
        this.listOfCallbackStarted = [];
        this.listOfCallbackStopped = [];
        this.messagesList = [];
        this.socket = this.createWebSocket();
    }

    private createWebSocket(): WebSocket {
        const socket = new WebSocket(this.url);

        socket.addEventListener('open', () => {
            console.log('Connected to the WebSocket server');
            openPopupInfo('Connected!');

            // Clear reconnect timer on successful connection
            if (this.reconnectTimer) {
                clearTimeout(this.reconnectTimer);
                this.reconnectTimer = null;
            }

            this.notifyCallbackStarted();
        });

        socket.addEventListener('message', (event) => {
            try {
                let frame = Protocol.decode(event.data);
                frame.direction = Direction.RX;
                frame.setDate();
                this.notifyRead(frame);

                if (this.messagesList.length > 0) {
                    if (frame.command?.name === ClusterGenericCommands.GENERIC ||
                        frame.cluster?.name === ClusterName.GENERIC ||
                        frame.cluster?.code === this.messagesList[0].cluster?.code) {

                        if (frame.command?.name === ClusterGenericCommands.GENERIC ||
                            frame.cluster?.name === ClusterName.GENERIC) {
                            openPopupWarning(getErrorName(frame.params[0]) + ': ' + this.messagesList[0].raw);
                        }
                        this.messagesList.shift();
                        this.updateProgressBar();
                        if (this.messagesList.length > 0) {
                            this.writeOnSocket(this.messagesList[0]).then();
                        }
                    }
                }
            } catch (error) {
                console.error('Error decoding message:', error);
                openPopupError(`Message decode error: ${error instanceof Error ? error.message : 'Unknown error'}`);
                return;
            }
        });

        socket.addEventListener('close', () => {
            console.log('Disconnected from the WebSocket server');
            openPopupWarning('WebSocket disconnected!');
            this.notifyCallbackStopped();
            this.messagesList = [];

            // Attempt reconnection if enabled
            if (this.shouldReconnect) {
                this.scheduleReconnect();
            }
        });

        socket.addEventListener('error', (event) => {
            console.error('WebSocket error:', event);
            openPopupError('WebSocket error!');
        });

        return socket;
    }

    private scheduleReconnect() {
        if (this.reconnectTimer) {
            return; // Already scheduled
        }

        console.log(`Reconnecting in ${RECONNECT_INTERVAL_MS}ms...`);
        this.reconnectTimer = setTimeout(() => {
            if (this.shouldReconnect && this.socket.readyState !== WebSocket.OPEN && this.socket.readyState !== WebSocket.CONNECTING) {
                console.log('Attempting to reconnect...');
                clearAllPopups();
                this.socket = this.createWebSocket();
            }
            this.reconnectTimer = null;
        }, RECONNECT_INTERVAL_MS);
    }

    disconnect() {
        this.shouldReconnect = false;
        if (this.reconnectTimer) {
            clearTimeout(this.reconnectTimer);
            this.reconnectTimer = null;
        }
        if (this.socket) {
            this.socket.close();
        }
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
            this.listOfSpecificCallbackRead.push({ callback: cb, cluster: cluster, command: command });
        } else {
            this.listOfSpecificCallbackRead.push({ callback: cb, cluster: cluster, command: command, params: params });
        }
    }

    addCallbackWrite(cb: Callback) {
        this.listOfCallbackWrite.push(cb);
    }

    notifyRead(message: Message) {
        console.log(`Received: ${message}`);
        this.listOfCallbackRead.forEach((cb) => {
            cb(message);
        });

        this.listOfSpecificCallbackRead.forEach((speCb) => {
            if (message.cluster?.name === speCb.cluster) {
                if (message.command?.name === speCb.command) {
                    if (message.params && speCb.params) {
                        for (let i = 0; i < speCb.params.length; i++) {
                            if (message.params[i] !== speCb.params[i]) {
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

    notifyWrite(message: Message) {
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

    private updateProgressBar() {
        const pb = document.getElementById('progress-message-queue');
        const pbVal = document.getElementById('progress-message-queue-val');
        if (pb) {
            const percentage = Math.min((this.messagesList.length / MAX_MESSAGE_QUEUE_SIZE) * 100, 100);
            pb.setAttribute('style', `width: ${percentage}%`);
        }
        if (pbVal) {
            pbVal.innerText = this.messagesList.length.toString();
        }
    }

    write(message: Message) {
        if (this.messagesList.length >= MAX_MESSAGE_QUEUE_SIZE) {
            openPopupWarning('WebSocket Messages list full! Message dropped.');
            return;
        }

        this.messagesList.push(message);
        this.updateProgressBar();

        if (this.messagesList.length === 1) {
            this.writeOnSocket(message).catch((error) => {
                console.error('Failed to write message:', error);
                this.messagesList.shift();
                this.updateProgressBar();
            });
        }
    }
}
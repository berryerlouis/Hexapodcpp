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
    private static readonly DEBUG_LOGS = true;

    private listOfSpecificCallbackRead: SpecificCallback[];
    private specificCallbacksByKey: Map<string, SpecificCallback[]>;
    private listOfCallbackRead: Callback[];
    private listOfCallbackWrite: Callback[];
    private listOfCallbackStarted: CallbackStarted[];
    private listOfCallbackStopped: CallbackStopped[];
    private socket: WebSocket;
    private messagesList: Message[];
    private url: string | URL;
    private reconnectTimer: number | null = null;
    private shouldReconnect: boolean = true;
    private pb: HTMLElement | null;
    private pbVal: HTMLElement | null;
    private lastProgressPercentage: number;
    private lastProgressCount: number;

    constructor(url: string | URL) {
        this.url = url;
        this.listOfCallbackRead = [];
        this.listOfCallbackWrite = [];
        this.listOfSpecificCallbackRead = [];
        this.specificCallbacksByKey = new Map<string, SpecificCallback[]>();
        this.listOfCallbackStarted = [];
        this.listOfCallbackStopped = [];
        this.messagesList = [];
        this.pb = document.getElementById('progress-message-queue');
        this.pbVal = document.getElementById('progress-message-queue-val');
        this.lastProgressPercentage = -1;
        this.lastProgressCount = -1;
        this.socket = this.createWebSocket();
    }

    private static log(message?: any, ...optionalParams: any[]) {
        if (Socket.DEBUG_LOGS) {
            console.log(message, ...optionalParams);
        }
    }

    private getSpecificCallbackKey(cluster: ClusterName, command: CommandName): string {
        return `${cluster}:${command}`;
    }

    private createWebSocket(): WebSocket {
        const socket = new WebSocket(this.url);

        socket.addEventListener('open', () => {
            Socket.log('Connected to the WebSocket server');
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
            Socket.log('Disconnected from the WebSocket server');
            openPopupWarning('WebSocket disconnected!');
            this.notifyCallbackStopped();
            this.messagesList = [];

            // Attempt reconnection if enabled
            if (this.shouldReconnect) {
                this.scheduleReconnect();
            }
        });

        socket.addEventListener('error', (event) => {
            if (Socket.DEBUG_LOGS) {
                console.error('WebSocket error:', event);
            }
            openPopupError('WebSocket error!');
        });

        return socket;
    }

    private scheduleReconnect() {
        if (this.reconnectTimer) {
            return; // Already scheduled
        }

        Socket.log(`Reconnecting in ${RECONNECT_INTERVAL_MS}ms...`);
        this.reconnectTimer = setTimeout(() => {
            if (this.shouldReconnect && this.socket.readyState !== WebSocket.OPEN && this.socket.readyState !== WebSocket.CONNECTING) {
                Socket.log('Attempting to reconnect...');
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
        const specificCallback = params == null
            ? { callback: cb, cluster: cluster, command: command }
            : { callback: cb, cluster: cluster, command: command, params: params };

        this.listOfSpecificCallbackRead.push(specificCallback);

        const callbackKey = this.getSpecificCallbackKey(cluster, command);
        const callbackBucket = this.specificCallbacksByKey.get(callbackKey);
        if (callbackBucket) {
            callbackBucket.push(specificCallback);
        } else {
            this.specificCallbacksByKey.set(callbackKey, [specificCallback]);
        }
    }

    addCallbackWrite(cb: Callback) {
        this.listOfCallbackWrite.push(cb);
    }

    notifyRead(message: Message) {
        Socket.log(`Received: ${message}`);
        this.listOfCallbackRead.forEach((cb) => {
            cb(message);
        });

        const clusterName = message.cluster?.name;
        const commandName = message.command?.name;
        if (clusterName && commandName) {
            const callbackKey = this.getSpecificCallbackKey(clusterName, commandName);
            const callbackBucket = this.specificCallbacksByKey.get(callbackKey);
            if (!callbackBucket) {
                return;
            }

            callbackBucket.forEach((speCb) => {
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
            });
        }
    }

    notifyWrite(message: Message) {
        message.setDate();
        Socket.log(`Transmit: ${message}`);
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
        const percentage = Math.min((this.messagesList.length / MAX_MESSAGE_QUEUE_SIZE) * 100, 100);
        const count = this.messagesList.length;

        if (this.pb && percentage !== this.lastProgressPercentage) {
            this.pb.style.width = `${percentage}%`;
            this.lastProgressPercentage = percentage;
        }
        if (this.pbVal && count !== this.lastProgressCount) {
            this.pbVal.innerText = count.toString();
            this.lastProgressCount = count;
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
import { CodeCommandNack } from "./Cluster.js";
import { Message } from "./Message.js";
import { SocketInterface } from "./Socket.js";

type Callback = (message: Message) => void;

interface MessageTx {
    message: Message;
    cbResponse?: Callback;
}

export class MessageManager {
    private socketInterface: SocketInterface;
    private listMessagesToSent: MessageTx[];
    private currentMessagesToSent: MessageTx | null;
    private listOfCallbackNotifyOnSpecificCommand: { clusterName: string, commandName: string, cb: Callback }[];
    private listOfCallbackRead: Callback[];
    private listOfCallbackReadAsync: Callback[];
    private listOfCallbackWrite: Callback[];
    private listOfCallbackTimeout: Callback[];

    constructor(socketInterface: SocketInterface) {
        this.socketInterface = socketInterface;
        this.listMessagesToSent = [];
        this.currentMessagesToSent = null;
        this.listOfCallbackNotifyOnSpecificCommand = [];
        this.listOfCallbackRead = [];
        this.listOfCallbackReadAsync = [];
        this.listOfCallbackWrite = [];
        this.listOfCallbackTimeout = [];
        this.socketInterface.addCallbackRead(this.read.bind(this));
    }

    write(message: Message, cbResponse?: Callback) {
        this.listMessagesToSent.unshift({ message, cbResponse });
    }

    read(message: Message) {
        message.setDate();
        let asyncMessageIncoming = true;
        if (this.currentMessagesToSent) {
            if (message.cluster.code === this.currentMessagesToSent.message.cluster.code) {
                asyncMessageIncoming = false;
                if (message.command.code === this.currentMessagesToSent.message.command.code) {
                    this.currentMessagesToSent.cbResponse && this.currentMessagesToSent.cbResponse(message);
                    this.currentMessagesToSent = null;
                } else if (message.command.code === CodeCommandNack) {
                    this.currentMessagesToSent.cbResponse && this.currentMessagesToSent.cbResponse(message);
                    this.currentMessagesToSent = null;
                }
            }
        }

        this.notifyOnSpecificCommand(message);
        if (asyncMessageIncoming) {
            this.notifyReadAsync(message);
        } else {
            this.notifyRead(message);
        }
    }

    update() {
        if (this.socketInterface.isConnected() === true) {
            // dequeue the list of messages to write
            if (this.listMessagesToSent.length > 0 && this.currentMessagesToSent == null) {
                const messageTx = this.listMessagesToSent.pop()!;
                messageTx.message.setDate();

                this.socketInterface.write(messageTx.message).then((response) => {
                    if(response === true)
                    {
                        this.currentMessagesToSent = messageTx;
                        this.notifyWrite(this.currentMessagesToSent);
                    }
                }).catch((error) => {
                    console.error("Message manager, can't write to socket:" + error);
                });
            }
            // waiting for response : timeout
            else if (this.currentMessagesToSent != null) {
                this.currentMessagesToSent.message.timeout++;
                if (this.currentMessagesToSent.message.timeout >= 100) {
                    this.currentMessagesToSent.cbResponse && this.currentMessagesToSent.cbResponse(this.currentMessagesToSent.message);
                    this.currentMessagesToSent = null;
                }
            }
        }
    }

    addCallbackNotifyOnSpecificCommand(clusterName: string, commandName: string, cb: Callback) {
        this.listOfCallbackNotifyOnSpecificCommand.push({ clusterName, commandName, cb });
    }

    addCallbackRead(cb: Callback) {
        this.listOfCallbackRead.push(cb);
    }

    addCallbackReadAsync(cb: Callback) {
        this.listOfCallbackReadAsync.push(cb);
    }

    addCallbackWrite(cb: Callback) {
        this.listOfCallbackWrite.push(cb);
    }

    addCallbackWriteTimeout(cb: Callback) {
        this.listOfCallbackTimeout.push(cb);
    }

    notifyOnSpecificCommand(message: any) {
        this.listOfCallbackNotifyOnSpecificCommand.forEach((specificCommand) => {
            message.index = 0;
            if ((specificCommand.clusterName === message.cluster.name) &&
                (specificCommand.commandName === message.command.name)) {
                specificCommand.cb && specificCommand.cb(message);
            }
        });
    }

    notifyRead(message: any) {
        this.listOfCallbackRead.forEach(function (cb) {
            message.index = 0;
            cb(message);
        });
    }

    notifyReadAsync(message: any) {
        this.listOfCallbackReadAsync.forEach(function (cb) {
            message.index = 0;
            cb(message);
        });
    }

    notifyWrite(message: any) {
        this.listOfCallbackWrite.forEach(function (cb) {
            message.index = 0;
            cb(message.message);
        });
    }

    notifyWriteTimeout(message: any) {
        this.listOfCallbackTimeout.forEach(function (cb) {
            message.index = 0;
            cb(message.message);
        });
    }
}
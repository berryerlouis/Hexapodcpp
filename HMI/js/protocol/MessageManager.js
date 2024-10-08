import {CodeCommandNack} from "./Cluster.js";

export class MessageManager {
    constructor(serialInterface) {
        this.serialInterface = serialInterface;
        this.listMessagesToSent = [];
        this.currentMessagesToSent = null;
        this.listOfCallbackNotifyOnSpecificCommand = [];
        this.listOfCallbackRead = [];
        this.listOfCallbackReadAsync = [];
        this.listOfCallbackWrite = [];
        this.listOfCallbackTimeout = [];
        this.serialInterface.addCallbackRead(this.read.bind(this));
    }

    write(message, cbResponse) {
        this.listMessagesToSent.unshift({message, cbResponse});
    }

    read(message) {
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
        if (this.serialInterface.initialized === true) {
            // dequeue the list of messages to write
            if (this.listMessagesToSent.length > 0 && this.currentMessagesToSent == null) {
                const messageTx = this.listMessagesToSent.pop();
                messageTx.message.setDate();
                if (this.serialInterface.write(messageTx.message)) {
                    this.currentMessagesToSent = messageTx;
                    this.notifyWrite(this.currentMessagesToSent);
                }
            }
            // waiting for response : timeout
            else if (this.currentMessagesToSent != null) {
                this.currentMessagesToSent.message.timeout++;
                if (this.currentMessagesToSent.message.timeout >= 10) {
                    this.currentMessagesToSent.cbResponse && this.currentMessagesToSent.cbResponse(this.currentMessagesToSent.message);

                    //retry
                    this.currentMessagesToSent.message.retry++;
                    if (this.currentMessagesToSent.message.retry < 5) {
                        this.listMessagesToSent.unshift(this.currentMessagesToSent);
                        console.warn(this.currentMessagesToSent.message.toString() + " Message timeout retry number (" + this.currentMessagesToSent.message.retry + ")");
                    } else {
                        this.notifyWriteTimeout(this.currentMessagesToSent);
                        console.error(this.currentMessagesToSent.message.toString() + " Timeout removed");
                        this.currentMessagesToSent = null
                    }
                }
            }
        }
    }

    addCallbackNotifyOnSpecificCommand(clusterName, commandName, cb) {
        this.listOfCallbackNotifyOnSpecificCommand.push({clusterName, commandName, cb});
    }

    addCallbackRead(cb) {
        this.listOfCallbackRead.push(cb);
    }

    addCallbackReadAsync(cb) {
        this.listOfCallbackReadAsync.push(cb);
    }

    addCallbackWrite(cb) {
        this.listOfCallbackWrite.push(cb);
    }

    addCallbackWriteTimeout(cb) {
        this.listOfCallbackTimeout.push(cb);
    }

    notifyOnSpecificCommand(message) {
        this.listOfCallbackNotifyOnSpecificCommand.forEach((specificCommand) => {
            message.index = 0;
            if ((specificCommand.clusterName === message.cluster.name) &&
                (specificCommand.commandName === message.command.name)) {
                specificCommand.cb && specificCommand.cb(message);
            }
        });
    }

    notifyRead(message) {
        this.listOfCallbackRead.forEach(function (cb) {
            message.index = 0;
            cb(message);
        });
    }

    notifyReadAsync(message) {
        this.listOfCallbackReadAsync.forEach(function (cb) {
            message.index = 0;
            cb(message);
        });
    }

    notifyWrite(message) {
        this.listOfCallbackWrite.forEach(function (cb) {
            message.index = 0;
            cb(message.message);
        });
    }

    notifyWriteTimeout(message) {
        this.listOfCallbackTimeout.forEach(function (cb) {
            message.index = 0;
            cb(message.message);
        });
    }

}
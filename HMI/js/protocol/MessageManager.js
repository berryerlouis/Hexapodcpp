export class MessageManager {
    constructor(serialInterface) {
        this.serialInterface = serialInterface;
        this.listMessagesToSent = [];
        this.currentMessagesToSent = null;
        this.listOfCallbackNotifyOnSpecificCommand = [];
        this.listOfCallbackRead = [];
        this.listOfCallbackWrite = [];
        this.listOfCallbackTimeout = [];
        this.serialInterface.addCallbackRead(this.read.bind(this));
    }

    write(message, cbResponse) {
        this.listMessagesToSent.push({message, cbResponse});
    }

    read(message) {
        message.setDate();
        if ((this.currentMessagesToSent != null) &&
            (message.cluster.code === this.currentMessagesToSent.message.cluster.code) &&
            (message.command.code === this.currentMessagesToSent.message.command.code)) {
            this.currentMessagesToSent.cbResponse && this.currentMessagesToSent.cbResponse(message);
            this.currentMessagesToSent = null;
        }
        this.notifyOnSpecificCommand(message);
        this.notifyRead(message);
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
                if (this.currentMessagesToSent.message.timeout >= 5) {
                    this.currentMessagesToSent.cbResponse && this.currentMessagesToSent.cbResponse(this.currentMessagesToSent.message);
                    this.notifyWriteTimeout(this.currentMessagesToSent);
                    this.currentMessagesToSent = null;
                    console.error("timeout");
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
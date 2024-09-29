import {Header} from './Header.js';
import {CodeCommandNack} from "../../protocol/Cluster.js";

export class Logger {
    constructor(messageManager) {
        this.console = $('#console');
        this.consoleList = $('#console-list');
        this.header = new Header(messageManager);
        this.initMessageManagerCallbacks(messageManager);
    }

    initMessageManagerCallbacks(messageManager) {
        this.messageManager = messageManager;
        this.messageManager.addCallbackRead((message) => {
            this.logMessage(message);
            this.header.incrementRxMessage();
        });
        this.messageManager.addCallbackReadAsync((message) => {
            this.logMessage(message, true);
            this.header.incrementRxMessage();
        });
        this.messageManager.addCallbackWrite((message) => {
            this.logMessage(message);
            this.header.incrementTxMessage();
        });
        this.messageManager.addCallbackWriteTimeout((message) => {
            this.header.incrementMessageTimeout();
            let itemLog = this.getLastTxMessage(message);
            this.setMessageAsTimeout(itemLog);
        });
    }

    setMessageAsTimeout(itemLog) {
        itemLog.childNodes[0].childNodes[1].childNodes[0].className = itemLog.childNodes[0].childNodes[1].childNodes[0].className.replace('-tx', '-tx-timeout');
    }

    getLastTxMessage(message) {
        return Array.from(this.consoleList[0].children).find((msg) => msg.message === message);
    }

    logMessage(message, async = false) {
        if (this.consoleList[0].children.length >= 2500) {
            this.consoleList[0].children[this.consoleList[0].children.length - 1].remove();
            //this.consoleList[0].children[0].remove();
        }

        let itemLog = document.createElement("li");
        itemLog.className = "list-group-item py-0";
        let itemLogRow = document.createElement("div");
        itemLogRow.className = "row w-100";

        const itemLogDate = this.setDate(message);
        const itemLogDirection = this.setDirection(message, async);
        const itemLogMessage = this.setMessage(message);
        const itemLogCluster = this.setCluster(message);
        const itemLogCommand = this.setCommand(message);
        const itemLogParams = this.setParams(message);

        itemLogRow.appendChild(itemLogDate);
        itemLogRow.appendChild(itemLogDirection);
        itemLogRow.appendChild(itemLogMessage);
        itemLogRow.appendChild(itemLogCluster);
        itemLogRow.appendChild(itemLogCommand);
        itemLogRow.appendChild(itemLogParams);
        itemLog.appendChild(itemLogRow);
        itemLog.message = message;
        this.consoleList.prepend(itemLog);

        //this.updateScrollView();

    }

    updateScrollView() {
        if ((this.console.prop("scrollHeight") - this.console.prop("scrollTop")) < 25) {
            this.console.animate({scrollTop: this.console.prop("scrollHeight") + this.console.prop("offsetHeight")}, 100);
        } else {
            this.console.animate({scrollTop: this.console.prop("scrollHeight") + this.console.prop("offsetHeight")}, 1);
        }
    }

    setDate(message) {
        const itemLogDate = document.createElement("div");
        itemLogDate.className = "col-2 px-0";
        const itemLogDateSpan = document.createElement("span");
        itemLogDateSpan.className = "console-date badge badge-pill";
        itemLogDateSpan.innerText = message.date;
        itemLogDate.appendChild(itemLogDateSpan);
        return itemLogDate;
    }

    setDirection(message, async) {
        const itemLogDirection = document.createElement("div");
        itemLogDirection.className = "col-1 px-0";
        const itemLogDirectionSpan = document.createElement("span");
        itemLogDirectionSpan.className = "console-direction"
            + (message.direction === "Rx" ? "-rx" : "-tx")
            + ((async && message.command.code !== CodeCommandNack) ? "-async" : "")
            + (message.command.code === CodeCommandNack ? "-nack" : "")
            + " badge badge-pill"
            + ((!async && message.direction === "Rx") ? " ms-2" : "");
        itemLogDirectionSpan.innerText = message.direction;
        itemLogDirection.appendChild(itemLogDirectionSpan);
        return itemLogDirection;
    }

    setMessage(message) {
        const itemLogMessage = document.createElement("div");
        itemLogMessage.className = "col-3 px-0";
        const itemLogMessageSpan = document.createElement("span");
        itemLogMessageSpan.className = "console-raw badge badge-pill text-wrap text-break mx-2";
        itemLogMessageSpan.innerText = message.raw.toUpperCase();
        itemLogMessage.appendChild(itemLogMessageSpan);
        return itemLogMessage;
    }

    setCluster(message) {
        const itemLogCluster = document.createElement("div");
        itemLogCluster.className = "col-1 px-0";
        const itemLogClusterSpan = document.createElement("span");
        itemLogClusterSpan.className = "console-cluster badge badge-pill";
        itemLogClusterSpan.innerText = message.cluster?.name.toUpperCase();
        itemLogCluster.appendChild(itemLogClusterSpan);
        return itemLogCluster;
    }

    setCommand(message) {
        const itemLogCommand = document.createElement("div");
        itemLogCommand.className = "col-1 px-0";
        const itemLogCommandSpan = document.createElement("span");
        itemLogCommandSpan.className = "console-cluster badge badge-pill";
        itemLogCommandSpan.innerText = message.command?.name.toUpperCase();
        itemLogCommand.appendChild(itemLogCommandSpan);
        return itemLogCommand;
    }

    setParams(message) {
        const itemLogParams = document.createElement("div");
        itemLogParams.className = "col-4 px-0";
        if (message.size > 0) {
            const itemLogParamsSpan = document.createElement("span");
            itemLogParamsSpan.className = "console-param badge badge-pill text-wrap ms-3";
            let params = "(" + message.size + ") [";
            for (let index = 0; index < message.params?.length; index++) {
                let intData = parseInt(message.params[index], 16);
                params += '0x' + intData.toString(16).padStart(2, "0").toUpperCase();
                params += (index < (message.params?.length - 1)) ? ' ' : '';
            }
            params += "]";
            itemLogParamsSpan.innerText = params;
            itemLogParams.appendChild(itemLogParamsSpan);
        }
        return itemLogParams;
    }
}




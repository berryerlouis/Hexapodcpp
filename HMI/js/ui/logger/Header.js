import {Protocol} from '../../protocol/Protocol.js';

export class Header {
    consoleList = undefined;

    constructor(messageManager) {
        this.messageManager = messageManager;
        this.initConsoleDropDown();
        this.initConsoleSendFrame();
        this.initConsoleClearButton();
        this.initConsoleStatMessage();
    }

    initConsoleDropDown() {
        this.consoleButtonDropDown = $('#console-button');
        this.consoleButtonDropDownIcon = $('#console-button-icon');
        this.console = $('#console');

        let bsCollapse = new bootstrap.Collapse($('#console'), {
            toggle: true
        })
        this.consoleButtonDropDown.click(() => {
            if (!this.console.hasClass('show')) {
                this.consoleButtonDropDownIcon.removeClass('bi-arrow-up');
                this.consoleButtonDropDownIcon.addClass('bi-arrow-down');
            } else {
                this.consoleButtonDropDownIcon.removeClass('bi-arrow-down');
                this.consoleButtonDropDownIcon.addClass('bi-arrow-up');
            }
            bsCollapse.toggle();
        });
    }

    initConsoleSendFrame() {
        this.frameToSend = $('#console-send-frame');
        this.consoleButtonframeToSend = $('#console-send-button');
        this.consoleListButtonShowHide = $('#console-send-frame-invalid');
        this.consoleListButtonShowHide.hide();
        this.consoleButtonframeToSend.click(() => {
            try {
                const parsedFrame = Protocol.decode("Tx", this.frameToSend.val());
                if (parsedFrame) {
                    this.messageManager.write(parsedFrame);
                }
            } catch (error) {
                this.showMessageParsingError(error);
            }
        });
        this.frameToSend.keypress((e) => {
            if (e.key === 'Enter') {
                try {
                    const parsedFrame = Protocol.decode("Tx", this.frameToSend.val());
                    if (parsedFrame) {
                        this.messageManager.write(parsedFrame);
                    }
                } catch (error) {
                    this.showMessageParsingError(error);
                }
            }
        })
    }

    showMessageParsingError(message) {
        this.consoleListButtonShowHide.text(message);
        this.consoleListButtonShowHide.fadeIn('fast');
        console.error(message);
        setTimeout(() => {
            this.consoleListButtonShowHide.fadeOut('slow');
        }, 5000);
    }

    initConsoleClearButton() {
        //clear console
        this.consoleListButtonClearConsoleList = $('#console-clear-button');
        this.consoleList = $('#console-list');
        this.consoleListButtonClearConsoleList.click(() => {
            this.consoleList[0].innerHTML = '';
            this.numberFrameSentRx[0].innerText = 0;
            this.numberFrameSentTx[0].innerText = 0;
            this.numberFrameSentTimeout[0].innerText = 0;
        });
    }

    initConsoleStatMessage() {
        this.numberFrameSentRx = $('#console-number-messages-rx');
        this.numberFrameSentTx = $('#console-number-messages-tx');
        this.numberFrameSentTimeout = $('#console-number-messages-timeout');
    }

    incrementRxMessage() {
        this.numberFrameSentRx[0].innerText = parseInt(this.numberFrameSentRx[0].innerText) + 1;
    }

    incrementTxMessage() {
        this.numberFrameSentTx[0].innerText = parseInt(this.numberFrameSentTx[0].innerText) + 1;
    }

    incrementMessageTimeout() {
        this.numberFrameSentTimeout[0].innerText = parseInt(this.numberFrameSentTimeout[0].innerText) + 1;
    }
}
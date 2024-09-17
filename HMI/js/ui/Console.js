import { Protocol } from '../protocol/Protocol.js';

export default class Console {
    consoleList = undefined;
    constructor() {
        this.stopScroll = false;
        this.console = $('#console');
        this.consoleButton = $('#console-button');
        this.consoleButtonIcon = $('#console-button-icon');
        this.frameToSend = $('#console-send-frame');
        this.numberFrameSentAll = $('#console-number-sent-frame-all');
        this.numberFrameSentTimeout = $('#console-number-sent-frame-timeout');
        this.numberFrameSentTx = $('#console-number-sent-frame-tx');
        this.consoleButtonSend = $('#console-send-button');
        this.consoleList = $('#console-list');
        this.consoleListButtonClear = $('#console-clear-button');
        this.consoleListButtonShowHide = $('#console-send-frame-invalid');
        this.consoleListButtonShowHide.hide();

        //clear console
        this.consoleListButtonClear.click(() => {
            this.clear();
        });
        this.consoleList.click(() => {
            this.stopScroll = (this.stopScroll == false) ? true : false;
        });


        //hide and show console
        let myCollapse = this.console
        let bsCollapse = new bootstrap.Collapse(myCollapse, {
            toggle: false
        })
        //bsCollapse.show();

        this.consoleButton.click(() => {
            if (!this.console.hasClass('show')) {
                this.consoleButtonIcon.removeClass('bi-arrow-up');
                this.consoleButtonIcon.addClass('bi-arrow-down');
            }
            else {
                this.consoleButtonIcon.removeClass('bi-arrow-down');
                this.consoleButtonIcon.addClass('bi-arrow-up');
            }
            bsCollapse.toggle();
        });

        //click send frame
        this.consoleButtonSend.click(() => {
            const parsedFrame = Protocol.decode(this.frameToSend.val(), this.showError.bind(this));
            if (parsedFrame) {
                this.cbSendRawMessage && this.cbSendRawMessage(parsedFrame);
            }
        });

        this.frameToSend.keypress((e) => {
            if (e.key == 'Enter') {
                const parsedFrame = Protocol.decode(this.frameToSend.val(), this.showError.bind(this));
                if (parsedFrame) {
                    this.cbSendRawMessage && this.cbSendRawMessage(parsedFrame);
                }
            }
        })
    }

    sendRawMessage(cbSendRawMessage) {
        this.cbSendRawMessage = cbSendRawMessage;
    }


    log(obj, message, response = false) {

        if (obj != "TIMEOUT") {
            let date = new Date();
            date = new Date(date.toString().split('GMT')[0] + ' UTC').toISOString().replace('T', ' ').replace('000Z', date.getMilliseconds().toString().padStart(3, '0'));

            let consoleItem = `<li class="list-group-item py-0">
                <div class="row w-100">
                    <div class="col-2">
                        <span class="console-date badge badge-pill">`+ date + `</span>
                    </div>
                    <div class="col-1">`
            consoleItem += response ? ' <i class="bi bi-arrow-return-right"></i>' : '';
            consoleItem += `<span class="console-direction`;
            consoleItem += obj == "RX" ? '-rx' : '';
            if (message.cluster) consoleItem += message.cluster.code == "FF" ? '-nack' : '';
            if (message.command) consoleItem += message.command.code == "FF" && message.params[0] == "00" ? '-nack' : '';
            if (obj == "RX") consoleItem += response ? '' : '-async';
            consoleItem += ` badge badge-pill">` + obj + `</span>
                    </div>
                    <div class="col-3">
                        <span class="console-raw badge badge-pill text-wrap text-break">`+ message.raw.toUpperCase() + `</span>
                    </div>
                    <div class="col-1">
                        <span class="console-cluster badge badge-pill">`;
            if (message.cluster) consoleItem += message.cluster && (message.cluster.name + ' (' + message.cluster.code + ')');
            consoleItem += `</span>
                    </div>
                    <div class="col-1">
                        <span class="console-command badge badge-pill">`
            if (message.command) consoleItem += message.command && (message.command.name + ' (' + message.command.code + ')');
            consoleItem += `</span>
                    </div>`;
            consoleItem += `<div class="col-4">`
            if (message.size && message.size > 0) {
                consoleItem += `<span class="console-param badge badge-pill text-wrap">(` + message.size + ') [';
                for (let index = 0; index < message.params?.length; index++) {
                    let intData = parseInt(message.params[index], 16);
                    consoleItem += '0x' + intData.toString(16).padStart(2, "0").toUpperCase();
                    consoleItem += (index < (message.params?.length - 1)) ? ' ' : '';
                }
                consoleItem += `]</span>`;
            }
            consoleItem += `</div></div></li>`;
            this.consoleList.append(consoleItem);
            if (this.consoleList[0].children.length > 500) {
                this.consoleList[0].children[0].remove();
            }
            if (this.stopScroll == false)
                this.consoleList[0].parentElement.scrollTop = this.consoleList[0].parentElement.scrollHeight;

            if (obj == "RESET")
                this.consoleList[0].children[this.consoleList[0].children.length - 1].classList.add('console-direction-rx-reset');
            if (obj == "ERROR")
                this.consoleList[0].children[this.consoleList[0].children.length - 1].classList.add('console-direction-rx-nack');
        }
        else {
            for (let index = this.consoleList[0].children.length - 1; index != 0; index--) {
                if (message.raw == this.consoleList[0].children[index].children[0].children[2].children[0].innerText) {
                    this.consoleList[0].children[index].classList.add('console-direction-rx-timeout');
                    this.numberFrameSentTimeout[0].innerText = parseInt(this.numberFrameSentTimeout[0].innerText) + 1;
                    break;
                }
            }
        }

        this.numberFrameSentAll[0].innerText = parseInt(this.numberFrameSentAll[0].innerText) + 1;
    }

    clear() {
        this.consoleList[0].innerHTML = '';
        this.numberFrameSentAll[0].innerText = 0;
        this.numberFrameSentTimeout[0].innerText = 0;
    }

    showError(message) {
        this.consoleListButtonShowHide.text(message);
        this.consoleListButtonShowHide.fadeIn('fast');
        setTimeout(() => {
            this.consoleListButtonShowHide.fadeOut('slow');
        }, 5000);
    }

}




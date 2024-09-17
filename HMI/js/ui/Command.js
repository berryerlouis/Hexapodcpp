
export class Command {
    constructor() {
        this.button = $('#open-command');
        this.close = $('#command-menu-close');
        this.menu = $('#command-menu');

        this.close.click(async () => {
            this.menu.hide();
        });
        this.button.click(async () => {
            this.menu.show();
        });
        this.menu.show();

    }

}


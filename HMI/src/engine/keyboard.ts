import Hexapod from "../entity/hexapod.ts";


export default class Keyboard {
    app: HTMLElement;
    hexapod: Hexapod

    constructor(hexapod: Hexapod) {
        this.app = document.getElementById('app')!;
        this.hexapod = hexapod;


        document.addEventListener('keydown', (e: KeyboardEvent) => {
            //console.log(e.code);
            switch (e.code) {
                case 'ArrowUp':
                    break;
                case 'ArrowDown':
                    break;
                case 'ArrowLeft':
                    break;
                case 'ArrowRight':
                    break;
                case 'Space':
                    //this.hexapod.translateZ(-0.01);
                    break;
            }
        });

    }
}
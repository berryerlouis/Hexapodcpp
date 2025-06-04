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
                    this.hexapod.MoveForward(-0.1);
                    break;
                case 'ArrowDown':
                    this.hexapod.MoveBackward(0.1);
                    break;
                case 'ArrowLeft':
                    this.hexapod.MoveLeft(0.1);
                    break;
                case 'ArrowRight':
                    this.hexapod.MoveRight(-0.1);
                    break;
                case 'Space':
                    //this.hexapod.translateZ(-0.01);
                    break;
            }
            this.logPositionRotation();
        });

    }

    logPositionRotation() {

        console.log(`Hexapod Position: x:${this.hexapod.position.x.toFixed(2)}, z:${this.hexapod.position.z.toFixed(2)}`);
        console.log(`Hexapod Rotation: x:${(this.hexapod.rotation.x * 180/Math.PI).toFixed(2)}, y:${(this.hexapod.rotation.y * 180/Math.PI).toFixed(2)}, z:${(this.hexapod.rotation.z * 180/Math.PI).toFixed(2)}`);
    }
}
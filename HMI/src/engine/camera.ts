import { PerspectiveCamera } from 'three'
import Hexapod from "../entity/hexapod.ts";

export default class Camera extends PerspectiveCamera {
    hexapod: Hexapod;
    needUpdate : boolean;
    constructor(hexapod: Hexapod) {
        super(70, innerWidth / innerHeight)
        this.hexapod = hexapod;
        this.needUpdate = false;
        this.hexapod.addCallbackMove(this.move.bind(this));
        this.position.set(0, 5.4, 5);
        this.lookAt(0, 0, 1.8);
    }

    move()  {
        this.needUpdate = false;
    }
    resize(aspectRation: number) {
        this.aspect = aspectRation;
        this.updateProjectionMatrix();
    }
    update() {
        if(this.needUpdate) {
            this.needUpdate = false;
            this.position.x = this.hexapod.position.x;
            this.position.z = this.hexapod.position.z + 5;
            this.lookAt(this.hexapod.position.x, 0, 1.8+this.hexapod.position.z)
        }
    }
}
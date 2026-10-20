import {PerspectiveCamera} from 'three'
import Hexapod from "../entity/hexapod.ts";

export default class Camera extends PerspectiveCamera {
    hexapod: Hexapod;
    needUpdate: boolean;

    constructor(hexapod: Hexapod) {
        super(50, innerWidth / innerHeight, 0.1, 200)
        this.hexapod = hexapod;
        this.needUpdate = false;
        this.hexapod.addCallbackMove(this.move.bind(this));
        this.position.set(2.8, 4.8, 4.4);
        this.lookAt(0, 0.9, 0);
    }

    move() {
        this.needUpdate = false;
    }

    resize(aspectRation: number) {
        this.aspect = aspectRation;
        this.updateProjectionMatrix();
    }

    update() {
        //if(this.needUpdate) {
        //this.lookAt(this.hexapod.position/*0, 0, 1.8*/);
        //this.position.set(this.hexapod.position.x, 5.4+this.hexapod.position.y, 5+this.hexapod.position.z)
        /*this.needUpdate = false;
        this.position.x = this.hexapod.position.x;
        this.position.z = this.hexapod.position.z + 5;
        this.lookAt(this.hexapod.position.x, 0, 1.8+this.hexapod.position.z)*/
        //}
    }
}
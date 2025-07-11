import {PerspectiveCamera} from 'three'
import Hexapod from "../entity/hexapod.ts";

export default class Camera extends PerspectiveCamera {
    hexapod: Hexapod;
    needUpdate: boolean;

    constructor(hexapod: Hexapod) {
        super(70, innerWidth / innerHeight)
        this.hexapod = hexapod;
        this.needUpdate = false;
        this.hexapod.addCallbackMove(this.move.bind(this));
        this.position.set(0, 10, 10);
        this.lookAt(this.hexapod.position);
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
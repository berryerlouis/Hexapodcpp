import {Color, FogExp2, Scene} from 'three'

export default class Display extends Scene {
    constructor() {
        super()
        this.background = new Color(0x0a1422);
        this.fog = new FogExp2(0x0a1422, 0.022);
    }

    update() {
    }
}
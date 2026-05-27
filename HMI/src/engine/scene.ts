import {Color, FogExp2, Scene} from 'three'

export default class Display extends Scene {
    constructor() {
        super()
        this.background = new Color(0x1a2533);
        this.fog = new FogExp2(0x1a2533, 0.003);
    }

    update() {
    }
}
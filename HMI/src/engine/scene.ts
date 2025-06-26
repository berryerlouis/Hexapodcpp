import {Color, FogExp2, Scene} from 'three'

export default class Display extends Scene {
    constructor() {
        super()
        this.background = new Color(0x515151);
        this.fog = new FogExp2(0xcccccc, 0.002);
    }

    update() {
    }
}
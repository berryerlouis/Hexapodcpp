import {DirectionalLight, HemisphereLight, Object3D} from 'three'
import Hexapod from "../entity/hexapod.ts";

export default class Light extends Object3D {
    constructor() {
        super()
        const hemisphere = new HemisphereLight('#a9c4e8', '#1b222c', 0.7);

        const key = new DirectionalLight('#fff1dc', 2.6);
        key.position.set(-5, 9, 6);
        key.castShadow = true;
        key.shadow.mapSize.set(2048, 2048);
        key.shadow.camera.left = -4.5;
        key.shadow.camera.right = 4.5;
        key.shadow.camera.top = 4.5;
        key.shadow.camera.bottom = -4.5;
        key.shadow.camera.near = 1;
        key.shadow.camera.far = 30;
        key.shadow.bias = -0.0004;
        key.shadow.normalBias = 0.02;

        const rim = new DirectionalLight('#5ab0ff', 1.6);
        rim.position.set(5, 4, -8);

        const fill = new DirectionalLight('#ffd7a8', 0.5);
        fill.position.set(7, 2, 5);

        this.add(hemisphere);
        this.add(key);
        this.add(key.target);
        this.add(rim);
        this.add(fill);
    }

    update(hexapod: Hexapod) {
        this.position.copy(hexapod.position);
    }
}

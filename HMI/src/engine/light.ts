import {AmbientLight, PointLight, Object3D, Vector2} from 'three'
import Hexapod from "../entity/hexapod.ts";

export default class Light extends Object3D {
    constructor() {
        super()
        const ambient = new AmbientLight('#FFFFFF', 0.7);
        const point = new PointLight('#FFFFFF', 80, 30);
        point.position.set(-4, 6, 6);
        point.castShadow = true;
        point.shadow.bias = -0.001;
        point.shadow.mapSize = new Vector2(2048, 2048);

        this.add(ambient);
        this.add(point)
    }

    update(hexapod: Hexapod) {
        this.position.copy(hexapod.position);
    }
}
import {AmbientLight, DirectionalLight, PointLight, Object3D, Vector2} from 'three'
import Hexapod from "../entity/hexapod.ts";

export default class Light extends Object3D {
    constructor() {
        super()
        // Soft fill for overall visibility
        const ambient = new AmbientLight('#FFFFFF', 1.4);

        // Key light — main illumination from top-front-left
        const keyLight = new PointLight('#FFFFFF', 120, 40);
        keyLight.position.set(-4, 9, 5);
        keyLight.castShadow = true;
        keyLight.shadow.bias = -0.001;
        keyLight.shadow.mapSize = new Vector2(2048, 2048);

        // Fill light — warm tone from opposite side to reduce harsh shadows
        const fillLight = new PointLight('#c8d8f0', 40, 30);
        fillLight.position.set(5, 4, -3);

        // Rim light — cool blue from behind for silhouette definition
        const rimLight = new PointLight('#4a90d9', 25, 25);
        rimLight.position.set(0, -1, -7);

        // Ground bounce — subtle warm uplight
        const bounceLight = new DirectionalLight('#ffd580', 0.3);
        bounceLight.position.set(0, -5, 0);
        bounceLight.target.position.set(0, 0, 0);

        this.add(ambient);
        this.add(keyLight);
        this.add(fillLight);
        this.add(rimLight);
        this.add(bounceLight);
        this.add(bounceLight.target);
    }

    update(hexapod: Hexapod) {
        this.position.copy(hexapod.position);
    }
}
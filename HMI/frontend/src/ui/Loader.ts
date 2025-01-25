import * as THREE from 'three';
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';

class SceneLoader {
    private loader: GLTFLoader;

    constructor() {
        this.loader = new GLTFLoader();
    }

    async loadModel(url: string): Promise<GLTFLoader.GLTF> {
        return new Promise((resolve, reject) => {
            this.loader.load(
                url,
                (gltf: GLTFLoader.GLTF) => {
                    resolve(gltf);
                },
                undefined,
                (error: any) => {
                    reject(error);
                }
            );
        });
    }


}

export default SceneLoader;
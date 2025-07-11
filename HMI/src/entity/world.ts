import {Mesh, Object3D, GridHelper, MeshLambertMaterial, BoxGeometry, Group} from 'three'
import Hexapod from "./hexapod.ts";

export default class World extends Object3D {

    hexapod: Hexapod;
    worldPlane: Group;
    width: number;
    height: number;
    meshes: Mesh[] = [];
    gridHelper;
    geometry = new BoxGeometry(1, 1, 0.1, 10, 10, 1);
    material = new MeshLambertMaterial({color: "#07bc13"});

    constructor(width: number, height: number, hexapod: Hexapod) {
        super()
        this.hexapod = hexapod;
        this.width = width;
        this.height = height;
        this.worldPlane = new Group();
        this.gridHelper = new GridHelper(width, height);
        this.geometry.translate(0, 0, -.051);
        this.initVisual(this.worldPlane);
        this.initGrid(this.gridHelper);
    }

    initVisual(worldPlane: Group) {
        this.meshes = [];
        for (let i = 0; i < this.width; i++) {
            for (let j = 0; j < this.height; j++) {
                const mesh = new Mesh(this.geometry, this.material);
                mesh.rotation.x = -Math.PI / 2;
                mesh.position.x = -(this.width / 2) + 1 / 2 + j;
                mesh.position.z = (this.height / 2) - 1 / 2 - i;
                mesh.castShadow = true
                mesh.receiveShadow = true;
                this.meshes.push(mesh);
            }
        }

        worldPlane.position.set(0, 0, 0)
        worldPlane.castShadow = true
        worldPlane.receiveShadow = true;
        worldPlane.add(...this.meshes);
        this.add(worldPlane);
    }

    initGrid(grid: GridHelper) {
        this.add(grid)
    }

    moveWorld(x: number, z: number) {
        if (Math.abs(this.worldPlane.position.z - (this.height / 2) - (z - (this.height / 2))) > 1) {
            if (this.worldPlane.position.z - (this.height / 2) - (z - (this.height / 2)) > 1) {
                this.worldPlane.position.z -= 1;
                this.gridHelper.position.z -= 1;
            } else if (this.worldPlane.position.z - (this.height / 2) - (z - (this.height / 2)) < -1) {
                this.worldPlane.position.z += 1;
                this.gridHelper.position.z += 1;
            }
        }
        if (Math.abs(this.worldPlane.position.x - (this.width / 2) - (x - (this.width / 2))) > 1) {
            if (this.worldPlane.position.x - (this.width / 2) - (x - (this.width / 2)) > 1) {
                this.worldPlane.position.x -= 1;
                this.gridHelper.position.x -= 1;
            } else if (this.worldPlane.position.x - (this.width / 2) - (x - (this.width / 2)) < -1) {
                this.worldPlane.position.x += 1;
                this.gridHelper.position.x += 1;
            }
        }
    }

    update() {
        this.moveWorld(this.hexapod.position.x, this.hexapod.position.z);
    }
}
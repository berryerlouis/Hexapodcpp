import {PlaneGeometry, Mesh, Object3D, GridHelper, MeshLambertMaterial} from 'three'

export default class World extends Object3D {
    constructor(width:number, height:number) {
        super()

        const geometry = new PlaneGeometry( width, height, 10, 10 );
        const material = new MeshLambertMaterial( { color: "#07bc13" } );
        const cube = new Mesh(geometry, material);
        const gridHelper = new GridHelper( width, height );
        this.initVisual(cube);
        this.initGrid(gridHelper);
    }

    initVisual(mesh:Mesh) {
        mesh.position.set(0, 0, 0)
        mesh.rotation.x = -Math.PI/2;
        mesh.castShadow = true
        this.add(mesh)
    }

    initGrid(grid:GridHelper) {
        this.add(grid)
    }

    update() {

    }
}
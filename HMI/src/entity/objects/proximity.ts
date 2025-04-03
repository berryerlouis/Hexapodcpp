import {BoxGeometry, DoubleSide, Mesh, MeshLambertMaterial, Object3D} from "three";

export enum ProximitySide {
    left,
    right,
    center
}
export default class ProximityObject extends Object3D {
    x: number;
    y: number;
    z: number;
    proximityLeft :Mesh;
    proximityCenter :Mesh;
    proximityRight :Mesh;
    constructor() {
        super();
        this.x = 0;
        this.y = 0;
        this.z = 0;

        this.proximityLeft = this.createProximityObject();
        this.proximityLeft.geometry.translate(-1,1,0);
        this.proximityLeft.position.set(-0.1,0,-1);
        this.proximityLeft.rotation.set(0,-1,0);

        this.proximityCenter = this.createProximityObject();
        this.proximityCenter.geometry.translate(1,1,0);
        this.proximityCenter.position.set(0,0,-1);
        this.proximityCenter.rotation.set(0,Math.PI/2,0);

        this.proximityRight = this.createProximityObject();
        this.proximityRight.geometry.translate(1,1,0);
        this.proximityRight.position.set(0.1,0,-1);
        this.proximityRight.rotation.set(0,1,0);

        this.proximityLeft.visible = false;
        this.proximityCenter.visible = false;
        this.proximityRight.visible = false;
        this.add(this.proximityLeft);
        this.add(this.proximityCenter);
        this.add(this.proximityRight);
    }

    update() {
        /*this.proximityLeft.position.x -= 0.01;
        this.proximityLeft.position.z -= 0.01;
        console.log(this.proximityLeft.position.x);
        console.log(this.proximityLeft.position.z);*/
    }

    createProximityObject(): Mesh {
        let geometry = new BoxGeometry( 0.2, 1, 1);
        const material = new MeshLambertMaterial({color: '#4364b6'});
        material.side = DoubleSide;
        let proximity = new Mesh(geometry, material);
        proximity.userData = {interval: 0};
        return new Mesh(geometry, material);
    }

    show(distance: number, side:ProximitySide) {
        if(distance > 0 && distance < 30) {
            if(side == ProximitySide.left) {
                this.proximityLeft.visible = true;
                clearInterval(this.proximityLeft.userData.interval);
                this.proximityLeft.userData.interval = setTimeout(()=>{
                    this.proximityLeft.visible = false
                }, 1000);
            } else if(side == ProximitySide.center) {
                this.proximityCenter.visible = true;
                clearInterval(this.proximityCenter.userData.interval);
                this.proximityCenter.userData.interval = setTimeout(()=>{
                    this.proximityCenter.visible = false
                }, 1000);
            } else {
                this.proximityRight.visible = true;
                clearInterval(this.proximityRight.userData.interval);
                this.proximityRight.userData.interval = setTimeout(()=>{
                    this.proximityRight.visible = false
                }, 1000);
            }
        }

        switch (side) {
            case ProximitySide.left:
                this.proximityLeft.position.x = -0.1 - distance/10;
                this.proximityLeft.position.z = -1 - distance/10;
                break;
            case ProximitySide.center:
                this.proximityCenter.position.z = -1 - distance/10;
                break;
            case ProximitySide.right:
                this.proximityRight.position.x = -0.1 + distance/10;
                this.proximityRight.position.z = -1 - distance/10;
                break;
        }
    }
}
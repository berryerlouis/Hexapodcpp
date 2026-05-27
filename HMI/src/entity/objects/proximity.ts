import {BoxGeometry, Color, DoubleSide, Mesh, MeshLambertMaterial, Object3D, Vector3} from "three";
import Socket from "../../communication/socket";

export enum ProximitySide {
    left,
    right,
    center
}

export default class ProximityObject extends Object3D {
    x: number;
    y: number;
    z: number;
    worldTrailLayer: Object3D;
    leftWorldTrail: Mesh[];
    rightWorldTrail: Mesh[];
    centerWorldTrail: Mesh[][][]; // [i][j][traceIndex]
    socket: Socket;

    constructor(socket: Socket) {
        super();
        this.socket = socket;
        this.x = 0;
        this.y = 0;
        this.z = 0;

        this.worldTrailLayer = new Object3D();
        this.worldTrailLayer.name = "proximity-world-trail";

        this.leftWorldTrail = [];
        this.rightWorldTrail = [];
        this.centerWorldTrail = [];
        for (let i = 0; i < 8; i++) {
            this.centerWorldTrail[i] = [];
            for (let j = 0; j < 8; j++) {
                this.centerWorldTrail[i][j] = [];
            }
        }
        this.socket.addCallbackStarted(() => {
            this.resetTraces();
        });
    }

    update() {
        /*this.proximityLeft.position.x -= 0.01;
        this.proximityLeft.position.z -= 0.01;
        console.log(this.proximityLeft.position.x);
        console.log(this.proximityLeft.position.z);*/
    }

    createProximityObject(geometry: BoxGeometry): Mesh {
        const material = new MeshLambertMaterial({color: '#4364b6'});
        material.side = DoubleSide;
        return new Mesh(geometry, material);
    }

    private ensureWorldTrailLayer() {
        let root: Object3D = this;
        while (root.parent) {
            root = root.parent;
        }
        if (this.worldTrailLayer.parent !== root) {
            if (this.worldTrailLayer.parent) {
                this.worldTrailLayer.parent.remove(this.worldTrailLayer);
            }
            root.add(this.worldTrailLayer);
        }
    }

    private createTrailMarker(sizeX: number, sizeY: number, sizeZ: number, distanceCm: number): Mesh {
        const mesh = this.createProximityObject(new BoxGeometry(sizeX, sizeY, sizeZ));
        mesh.userData.distanceCm = distanceCm;
        return mesh;
    }

    static MAX_TRACE_DEPTH = 100;

    private pushMarker(queue: Mesh[], marker: Mesh) {
        this.ensureWorldTrailLayer();
        this.worldTrailLayer.add(marker);
        queue.push(marker);
        if (queue.length > ProximityObject.MAX_TRACE_DEPTH) {
            const removed = queue.shift();
            if (removed && removed.parent) {
                removed.parent.remove(removed);
            }
        }
    }

    private applyVisualStyle(mesh: Mesh, alpha: number) {
        const blue = new Color('#4364b6');
        const red = new Color('#ff2a2a');
        const distanceCm = typeof mesh.userData.distanceCm === 'number' ? mesh.userData.distanceCm : 100;
        const t = distanceCm < 100 ? Math.max(0, Math.min(1, (100 - distanceCm) / 100)) : 0;
        const color = blue.clone().lerp(red, t);

        const material = mesh.material as MeshLambertMaterial;
        material.color.copy(color);
        material.opacity = alpha;
        material.transparent = true;
    }

    private updateQueueOpacity(queue: Mesh[]) {
        const total = queue.length;
        for (let idx = 0; idx < total; idx++) {
            const ageNorm = total > 1 ? idx / (total - 1) : 0;
            const alpha = 0.15 + ageNorm * 0.85;
            this.applyVisualStyle(queue[idx], alpha);
            queue[idx].visible = true;
        }
    }

    private toWorldPoint(localX: number, localY: number, localZ: number): Vector3 {
        const world = new Vector3(localX, localY, localZ);
        this.localToWorld(world);
        return world;
    }

    show(distance: number | number[][], side: ProximitySide) {
        if (side === ProximitySide.center && Array.isArray(distance)) {
            for (let i = 0; i < 8; i++) {
                for (let j = 0; j < 8; j++) {
                    const dist = distance[i][j];
                    if (dist <= 0) {
                        continue;
                    }
                    const localX = (i - 3.5) * 0.5;
                    const localY = (j - 3.5) * 0.5;
                    const localZ = -1 - dist / 10;
                    const worldPosition = this.toWorldPoint(localX, localY, localZ);
                    const marker = this.createTrailMarker(0.08, 0.08, 0.08, dist);
                    marker.position.copy(worldPosition);
                    this.pushMarker(this.centerWorldTrail[i][j], marker);
                    this.updateQueueOpacity(this.centerWorldTrail[i][j]);
                }
            }
        } else if (typeof distance === 'number') {
            if (distance > 0) {
                if (side === ProximitySide.left) {
                    const range = distance / 10;
                    const worldPosition = this.toWorldPoint(-0.1 - (Math.sin(Math.PI / 3) * range), 0, -1 - (Math.cos(Math.PI / 3) * range));
                    const marker = this.createTrailMarker(0.12, 0.12, 0.12, distance);
                    marker.position.copy(worldPosition);
                    this.pushMarker(this.leftWorldTrail, marker);
                    this.updateQueueOpacity(this.leftWorldTrail);
                } else if (side === ProximitySide.right) {
                    const range = distance / 10;
                    const worldPosition = this.toWorldPoint(-0.1 + (Math.sin(Math.PI / 3) * range), 0, -1 - (Math.cos(Math.PI / 3) * range));
                    const marker = this.createTrailMarker(0.12, 0.12, 0.12, distance);
                    marker.position.copy(worldPosition);
                    this.pushMarker(this.rightWorldTrail, marker);
                    this.updateQueueOpacity(this.rightWorldTrail);
                }
            }
        }
    }

    // Reset all traces
    resetTraces() {
        for (let idx = 0; idx < this.leftWorldTrail.length; idx++) {
            const marker = this.leftWorldTrail[idx];
            if (marker.parent) {
                marker.parent.remove(marker);
            }
        }
        for (let idx = 0; idx < this.rightWorldTrail.length; idx++) {
            const marker = this.rightWorldTrail[idx];
            if (marker.parent) {
                marker.parent.remove(marker);
            }
        }
        this.leftWorldTrail = [];
        this.rightWorldTrail = [];

        for (let i = 0; i < 8; i++) {
            for (let j = 0; j < 8; j++) {
                const queue = this.centerWorldTrail[i][j];
                for (let idx = 0; idx < queue.length; idx++) {
                    const marker = queue[idx];
                    if (marker.parent) {
                        marker.parent.remove(marker);
                    }
                }
                this.centerWorldTrail[i][j] = [];
            }
        }

        if (this.worldTrailLayer.parent) {
            this.worldTrailLayer.parent.remove(this.worldTrailLayer);
        }
    }
}
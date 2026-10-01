import {
    CanvasTexture,
    Mesh,
    MeshStandardMaterial,
    Object3D,
    PlaneGeometry,
    RepeatWrapping,
    SRGBColorSpace,
} from 'three'
import Hexapod from "./hexapod.ts";

// The floor is one big plane that follows the hexapod in whole texture periods, so the grid
// stays fixed in world space and the bot appears to walk on an endless surface.
export default class World extends Object3D {
    private static readonly TILE = 4;

    hexapod: Hexapod;
    width: number;
    height: number;
    ground: Mesh;

    constructor(width: number, height: number, hexapod: Hexapod) {
        super()
        this.hexapod = hexapod;
        this.width = width;
        this.height = height;

        const size = Math.ceil((Math.max(width, height) * 8) / World.TILE) * World.TILE;
        const texture = World.makeTexture();
        texture.repeat.set(size / World.TILE, size / World.TILE);
        texture.anisotropy = 16;

        this.ground = new Mesh(
            new PlaneGeometry(size, size),
            new MeshStandardMaterial({ map: texture, roughness: 0.92, metalness: 0 }),
        );
        this.ground.rotation.x = -Math.PI / 2;
        this.ground.receiveShadow = true;
        this.add(this.ground);
    }

    private static makeTexture(): CanvasTexture {
        const cells = World.TILE;
        const cell = 128;
        const canvas = document.createElement('canvas');
        canvas.width = canvas.height = cells * cell;
        const ctx = canvas.getContext('2d')!;

        for (let i = 0; i < cells; i++) {
            for (let j = 0; j < cells; j++) {
                ctx.fillStyle = (i + j) % 2 === 0 ? '#16222f' : '#1a2837';
                ctx.fillRect(i * cell, j * cell, cell, cell);
            }
        }
        for (let i = 0; i < 4000; i++) {
            ctx.fillStyle = `rgba(255,255,255,${Math.random() * 0.035})`;
            ctx.fillRect(Math.random() * canvas.width, Math.random() * canvas.height, 2, 2);
        }

        ctx.strokeStyle = 'rgba(120, 170, 220, 0.16)';
        ctx.lineWidth = 2;
        for (let i = 0; i <= cells; i++) {
            ctx.beginPath();
            ctx.moveTo(i * cell, 0);
            ctx.lineTo(i * cell, canvas.height);
            ctx.moveTo(0, i * cell);
            ctx.lineTo(canvas.width, i * cell);
            ctx.stroke();
        }

        const texture = new CanvasTexture(canvas);
        texture.wrapS = RepeatWrapping;
        texture.wrapT = RepeatWrapping;
        texture.colorSpace = SRGBColorSpace;
        return texture;
    }

    update() {
        this.ground.position.x = Math.round(this.hexapod.position.x / World.TILE) * World.TILE;
        this.ground.position.z = Math.round(this.hexapod.position.z / World.TILE) * World.TILE;
    }
}

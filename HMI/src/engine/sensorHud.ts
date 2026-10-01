import { Vector3 } from "three";
import Socket from "../communication/socket.ts";
import Message from "../communication/message.ts";
import { ClusterName } from "../communication/clusters/clusterType.ts";
import { ClusterProximityCommands } from "../communication/clusters/clusterProximity.ts";
import Hexapod from "../entity/hexapod.ts";
import RangeMap from "../entity/objects/rangeMap.ts";
import RangeSimulator from "../entity/objects/rangeSimulator.ts";
import {
    CM_PER_UNIT,
    DEFAULT_THRESHOLD,
    LONG_RANGE_THRESHOLD,
    TOF,
    US,
    rangeCss,
} from "../entity/objects/rangeSensors.ts";

const COLLAPSE_KEY = 'hexapod.radarCollapsed';
const ZOOMS_M = [1.5, 3, 5, 8];
const REFRESH_MS = 66;
const SCOPE_SIZE = 204;
const TOF_CELL = 12;
const US_BAR_FULL_CM = 200;
const SENSOR_ID = { usLeft: 0, usRight: 1, tof: 2 };

const TEMPLATE = `
<header class="radar-head">
    <span class="radar-title"><i class="bi bi-bullseye"></i>Environment</span>
    <span class="radar-chip" data-role="chip" data-state="idle">No signal</span>
    <button type="button" class="radar-fold" data-role="fold" aria-label="Collapse sensors panel"><i class="bi bi-chevron-up"></i></button>
</header>
<div class="radar-body">
    <div class="radar-main">
        <div class="radar-scope">
            <canvas data-role="scope"></canvas>
            <button type="button" class="radar-zoom" data-role="zoom" title="Change range">3 m</button>
        </div>
        <div class="radar-side">
            <div class="radar-block">
                <span class="radar-label">ToF 8&times;8</span>
                <canvas data-role="tof"></canvas>
                <span class="radar-value">min <b data-role="tof-min">--</b></span>
            </div>
            <div class="radar-block">
                <span class="radar-label">Ultrasound</span>
                <div class="us-row"><span>L</span><div class="us-bar"><i data-role="us-left-bar"></i></div><b data-role="us-left">--</b></div>
                <div class="us-row"><span>R</span><div class="us-bar"><i data-role="us-right-bar"></i></div><b data-role="us-right">--</b></div>
            </div>
        </div>
    </div>
    <div class="radar-stats">
        <span><b data-role="area">0.0</b> m&sup2; explored</span>
        <span><b data-role="points">0</b> obstacle blocks</span>
    </div>
    <div class="radar-actions">
        <button type="button" class="radar-toggle" data-role="beams"><i class="bi bi-broadcast"></i>Beams</button>
        <button type="button" class="radar-toggle" data-role="range" title="Report objects up to 4 m instead of the firmware default"><i class="bi bi-arrows-expand"></i>Long range</button>
        <button type="button" class="radar-toggle" data-role="sim" title="Raycast a hidden virtual room instead of the real sensors"><i class="bi bi-box"></i>Sim room</button>
        <button type="button" class="radar-toggle" data-role="clear" title="Forget everything explored so far"><i class="bi bi-trash3"></i>Clear</button>
    </div>
</div>`;

// Top-down radar, 8x8 depth image and controls for the range sensors.
export default class SensorHud {
    private readonly root: HTMLElement;
    private readonly hexapod: Hexapod;
    private readonly map: RangeMap;
    private readonly simulator: RangeSimulator;
    private readonly socket: Socket;

    private readonly scope: HTMLCanvasElement;
    private readonly scopeContext: CanvasRenderingContext2D;
    private readonly tof: HTMLCanvasElement;
    private readonly tofContext: CanvasRenderingContext2D;
    private readonly pixelRatio: number;

    private zoomIndex = 1;
    private longRange = true;
    private lastRefresh = 0;
    private readonly forward = new Vector3();

    // World to scope projection, heading up.
    private cx = 0;
    private cy = 0;
    private scale = 1;
    private x0 = 0;
    private z0 = 0;
    private fx = 0;
    private fz = -1;

    constructor(root: HTMLElement, hexapod: Hexapod, map: RangeMap, simulator: RangeSimulator, socket: Socket, simulate: boolean) {
        this.root = root;
        this.hexapod = hexapod;
        this.map = map;
        this.simulator = simulator;
        this.socket = socket;
        this.pixelRatio = Math.min(window.devicePixelRatio || 1, 2);

        root.innerHTML = TEMPLATE;
        this.scope = this.find<HTMLCanvasElement>('scope');
        this.tof = this.find<HTMLCanvasElement>('tof');
        this.scopeContext = this.prepareCanvas(this.scope, SCOPE_SIZE, SCOPE_SIZE);
        this.tofContext = this.prepareCanvas(this.tof, TOF_CELL * TOF.size, TOF_CELL * TOF.size);

        if (localStorage.getItem(COLLAPSE_KEY) === '1') {
            root.classList.add('is-collapsed');
        }
        this.find('fold').addEventListener('click', () => {
            root.classList.toggle('is-collapsed');
            localStorage.setItem(COLLAPSE_KEY, root.classList.contains('is-collapsed') ? '1' : '0');
        });
        this.find('zoom').addEventListener('click', () => {
            this.zoomIndex = (this.zoomIndex + 1) % ZOOMS_M.length;
            this.find('zoom').textContent = `${ZOOMS_M[this.zoomIndex]} m`;
        });

        this.bindToggle('beams', this.proximity.liveBeams, (on) => {
            this.proximity.liveBeams = on;
        });
        this.bindToggle('range', this.longRange, (on) => {
            this.longRange = on;
            this.applyRange();
        });
        this.bindToggle('sim', simulate, (on) => {
            this.setSimulation(on);
        });
        this.find('clear').addEventListener('click', () => {
            this.proximity.resetTraces();
        });

        this.setSimulation(simulate);
        this.socket.addCallbackStarted(() => {
            this.applyRange();
        });
    }

    private get proximity() {
        return this.hexapod.head.proximity;
    }

    update(now: number) {
        if (this.root.classList.contains('is-collapsed') || now - this.lastRefresh < REFRESH_MS) {
            return;
        }
        this.lastRefresh = now;

        this.updateReadouts(now);
        this.drawTof();
        this.drawScope(now);
    }

    private find<T extends HTMLElement = HTMLElement>(role: string): T {
        return this.root.querySelector<T>(`[data-role="${role}"]`)!;
    }

    private prepareCanvas(canvas: HTMLCanvasElement, width: number, height: number): CanvasRenderingContext2D {
        canvas.width = Math.round(width * this.pixelRatio);
        canvas.height = Math.round(height * this.pixelRatio);
        canvas.style.width = `${width}px`;
        canvas.style.height = `${height}px`;
        return canvas.getContext('2d')!;
    }

    private bindToggle(role: string, initial: boolean, onChange: (on: boolean) => void) {
        const button = this.find<HTMLButtonElement>(role);
        button.classList.toggle('is-on', initial);
        button.addEventListener('click', () => {
            const on = !button.classList.contains('is-on');
            button.classList.toggle('is-on', on);
            onChange(on);
        });
    }

    private setSimulation(on: boolean) {
        this.simulator.enabled = on;
        this.proximity.source = on ? 'sim' : 'live';
        this.proximity.resetTraces();
        this.find('sim').classList.toggle('is-on', on);
    }

    // The firmware only reports objects closer than its threshold, which is too short to map a room.
    private applyRange() {
        if (!this.socket.isConnected()) {
            return;
        }
        const usCm = this.longRange ? LONG_RANGE_THRESHOLD.usCm : DEFAULT_THRESHOLD.usCm;
        const tofMm = this.longRange ? LONG_RANGE_THRESHOLD.tofMm : DEFAULT_THRESHOLD.tofMm;
        for (const [id, value] of [[SENSOR_ID.usLeft, usCm], [SENSOR_ID.usRight, usCm], [SENSOR_ID.tof, tofMm]]) {
            this.socket.write(new Message(ClusterName.PROXIMITY, ClusterProximityCommands.SET_THRESHOLD, [id, value], [0xFF, 0xFFFF]));
        }
    }

    private updateReadouts(now: number) {
        const p = this.proximity;
        const tofFresh = p.isTofFresh(now);
        const online = tofFresh || p.isUltrasoundFresh(0, now) || p.isUltrasoundFresh(1, now);
        const chip = this.find('chip');
        const state = !online ? 'idle' : this.simulator.enabled ? 'sim' : 'live';
        chip.dataset.state = state;
        chip.textContent = state === 'idle' ? 'No signal' : state === 'sim' ? 'Simulated' : 'Live';

        let min = Infinity;
        if (tofFresh) {
            for (const cm of p.tofCm) {
                if (cm > 0 && cm < min) {
                    min = cm;
                }
            }
        }
        this.find('tof-min').textContent = Number.isFinite(min) ? `${Math.round(min)} cm` : '--';

        const sides = [['us-left', 0], ['us-right', 1]] as const;
        for (const [role, index] of sides) {
            const cm = p.isUltrasoundFresh(index, now) ? p.ultrasoundCm[index] : 0;
            this.find(role).textContent = cm > 0 ? `${Math.round(cm)} cm` : '--';
            const bar = this.find<HTMLElement>(`${role}-bar`);
            bar.style.width = cm > 0 ? `${Math.max(4, (1 - Math.min(cm, US_BAR_FULL_CM) / US_BAR_FULL_CM) * 100)}%` : '0%';
            bar.style.background = cm > 0 ? rangeCss(cm) : 'transparent';
        }

        this.find('area').textContent = this.map.exploredAreaM2.toFixed(1);
        this.find('points').textContent = this.map.obstacleBlocks.toString();
    }

    private drawTof() {
        const ctx = this.tofContext;
        const size = TOF_CELL * TOF.size;
        const fresh = this.proximity.isTofFresh();
        ctx.setTransform(this.pixelRatio, 0, 0, this.pixelRatio, 0, 0);
        ctx.clearRect(0, 0, size, size);

        // Shown as seen from the sensor, whatever the firmware zone order is.
        for (let row = 0; row < TOF.size; row++) {
            for (let col = 0; col < TOF.size; col++) {
                const cm = fresh ? this.proximity.tofCm[row * TOF.size + col] : 0;
                const x = (TOF.flipColumns ? TOF.size - 1 - col : col) * TOF_CELL;
                const y = (TOF.flipRows ? TOF.size - 1 - row : row) * TOF_CELL;
                ctx.fillStyle = cm > 0 ? rangeCss(cm) : 'rgba(147, 169, 191, 0.1)';
                ctx.beginPath();
                ctx.roundRect(x + 0.75, y + 0.75, TOF_CELL - 1.5, TOF_CELL - 1.5, 2);
                ctx.fill();
            }
        }
    }

    private drawScope(now: number) {
        const ctx = this.scopeContext;
        const size = SCOPE_SIZE;
        const radius = size / 2 - 2;
        const rangeM = ZOOMS_M[this.zoomIndex];
        const p = this.proximity;

        this.hexapod.updateWorldMatrix(true, false);
        p.updateWorldMatrix(true, false);
        this.forward.set(0, 0, -1).transformDirection(p.matrixWorld);
        const length = Math.hypot(this.forward.x, this.forward.z) || 1;
        this.fx = this.forward.x / length;
        this.fz = this.forward.z / length;
        this.x0 = this.hexapod.position.x;
        this.z0 = this.hexapod.position.z;
        this.cx = size / 2;
        this.cy = size / 2;
        this.scale = radius / ((rangeM * 100) / CM_PER_UNIT);

        ctx.setTransform(this.pixelRatio, 0, 0, this.pixelRatio, 0, 0);
        ctx.clearRect(0, 0, size, size);
        ctx.save();
        ctx.beginPath();
        ctx.arc(this.cx, this.cy, radius, 0, Math.PI * 2);
        ctx.clip();

        const background = ctx.createRadialGradient(this.cx, this.cy, 0, this.cx, this.cy, radius);
        background.addColorStop(0, '#0f2236');
        background.addColorStop(1, '#08131f');
        ctx.fillStyle = background;
        ctx.fillRect(0, 0, size, size);

        this.drawMap(ctx);
        this.drawGrid(ctx, radius, rangeM);

        if (p.isTofFresh(now)) {
            this.drawTofView(ctx, p.tofOrigin);
        }
        for (let i = 0; i < 2; i++) {
            if (p.isUltrasoundFresh(i, now) && p.ultrasoundCm[i] > 0) {
                this.drawUltrasound(ctx, i);
            }
        }
        this.drawRobot(ctx);
        ctx.restore();

        ctx.strokeStyle = 'rgba(160, 200, 235, 0.35)';
        ctx.lineWidth = 1.5;
        ctx.beginPath();
        ctx.arc(this.cx, this.cy, radius, 0, Math.PI * 2);
        ctx.stroke();
    }

    private toScreen(x: number, z: number): [number, number] {
        const dx = x - this.x0;
        const dz = z - this.z0;
        const rx = -this.fz;
        const rz = this.fx;
        return [
            this.cx + this.scale * (rx * dx + rz * dz),
            this.cy - this.scale * (this.fx * dx + this.fz * dz),
        ];
    }

    // Draws the shared floor canvas through the same heading-up projection.
    private drawMap(ctx: CanvasRenderingContext2D) {
        const cell = RangeMap.CELL;
        const half = (RangeMap.GRID * cell) / 2;
        const [ox, oy] = this.toScreen(-half, -half);
        const [ex, ey] = this.toScreen(-half + cell, -half);
        const [fx, fy] = this.toScreen(-half, -half + cell);
        ctx.save();
        ctx.imageSmoothingEnabled = false;
        ctx.setTransform(
            (ex - ox) * this.pixelRatio, (ey - oy) * this.pixelRatio,
            (fx - ox) * this.pixelRatio, (fy - oy) * this.pixelRatio,
            ox * this.pixelRatio, oy * this.pixelRatio,
        );
        ctx.drawImage(this.map.canvas, 0, 0);
        ctx.restore();
    }

    private drawGrid(ctx: CanvasRenderingContext2D, radius: number, rangeM: number) {
        const step = rangeM <= 3 ? (rangeM <= 1.5 ? 0.5 : 1) : rangeM <= 5 ? 1 : 2;
        ctx.strokeStyle = 'rgba(160, 200, 235, 0.16)';
        ctx.fillStyle = 'rgba(147, 169, 191, 0.7)';
        ctx.lineWidth = 1;
        ctx.font = '9px "IBM Plex Mono", monospace';
        for (let m = step; m < rangeM; m += step) {
            const r = (radius * m) / rangeM;
            ctx.beginPath();
            ctx.arc(this.cx, this.cy, r, 0, Math.PI * 2);
            ctx.stroke();
            ctx.fillText(`${m}m`, this.cx + 3, this.cy - r - 2);
        }
        ctx.beginPath();
        ctx.moveTo(this.cx - radius, this.cy);
        ctx.lineTo(this.cx + radius, this.cy);
        ctx.moveTo(this.cx, this.cy - radius);
        ctx.lineTo(this.cx, this.cy + radius);
        ctx.stroke();
    }

    private drawTofView(ctx: CanvasRenderingContext2D, origin: Vector3) {
        const p = this.proximity;
        const [ox, oy] = this.toScreen(origin.x, origin.z);

        let far = 0;
        let near = Infinity;
        for (let i = 0; i < p.tofCm.length; i++) {
            if (p.tofCm[i] > 0) {
                far = Math.max(far, p.tofCm[i]);
                near = Math.min(near, p.tofCm[i]);
            }
        }
        if (far === 0) {
            return;
        }

        const half = (TOF.fovDeg * Math.PI) / 360;
        ctx.fillStyle = rangeCss(near);
        ctx.globalAlpha = 0.12;
        ctx.beginPath();
        ctx.moveTo(ox, oy);
        ctx.arc(ox, oy, (far / CM_PER_UNIT) * this.scale, -Math.PI / 2 - half, -Math.PI / 2 + half);
        ctx.closePath();
        ctx.fill();
        ctx.globalAlpha = 1;

        for (let i = 0; i < p.tofCm.length; i++) {
            if (p.tofCm[i] <= 0) {
                continue;
            }
            const [sx, sy] = this.toScreen(p.tofHits[i * 3], p.tofHits[i * 3 + 2]);
            ctx.fillStyle = rangeCss(p.tofCm[i]);
            ctx.fillRect(sx - 1.25, sy - 1.25, 2.5, 2.5);
        }
    }

    private drawUltrasound(ctx: CanvasRenderingContext2D, index: number) {
        const p = this.proximity;
        const [ox, oy] = this.toScreen(p.ultrasoundOrigin[index].x, p.ultrasoundOrigin[index].z);
        const [hx, hy] = this.toScreen(p.ultrasoundHit[index].x, p.ultrasoundHit[index].z);
        const angle = Math.atan2(hy - oy, hx - ox);
        const length = Math.hypot(hx - ox, hy - oy);
        const half = (US.halfAngleDeg * Math.PI) / 180;
        const color = rangeCss(p.ultrasoundCm[index]);

        ctx.fillStyle = color;
        ctx.globalAlpha = 0.2;
        ctx.beginPath();
        ctx.moveTo(ox, oy);
        ctx.arc(ox, oy, length, angle - half, angle + half);
        ctx.closePath();
        ctx.fill();
        ctx.globalAlpha = 1;

        ctx.strokeStyle = color;
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.arc(ox, oy, length, angle - half, angle + half);
        ctx.stroke();
    }

    private drawRobot(ctx: CanvasRenderingContext2D) {
        const r = Math.max(6, 1.6 * this.scale);
        ctx.beginPath();
        for (let i = 0; i < 6; i++) {
            const a = (Math.PI / 3) * i - Math.PI / 2;
            ctx.lineTo(this.cx + Math.cos(a) * r, this.cy + Math.sin(a) * r);
        }
        ctx.closePath();
        ctx.fillStyle = 'rgba(46, 230, 214, 0.22)';
        ctx.fill();
        ctx.strokeStyle = '#2ee6d6';
        ctx.lineWidth = 1.5;
        ctx.stroke();

        ctx.fillStyle = '#2ee6d6';
        ctx.beginPath();
        ctx.moveTo(this.cx, this.cy - r - 5);
        ctx.lineTo(this.cx - 4, this.cy - r + 1);
        ctx.lineTo(this.cx + 4, this.cy - r + 1);
        ctx.closePath();
        ctx.fill();
    }
}

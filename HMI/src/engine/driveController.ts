import Hexapod from "../entity/hexapod.ts";

/**
 * Merges analog drive inputs (virtual sticks, gamepad, point-and-go) into bot commands.
 *
 * Conventions: move x is right-positive and y is up-positive in [-1, 1]; turn is
 * clockwise-positive in [-1, 1]. Stick magnitude scales the stride amplitude.
 */
export default class DriveController {
    static readonly AUTO = 'auto';

    private static readonly DEADZONE = 0.12;
    private static readonly MAX_TURN_DEG = 30;
    private static readonly MIN_AMPLITUDE = 3;
    private static readonly DIRECTION_STEP_DEG = 5;
    private static readonly SEND_INTERVAL_MS = 80;
    private static readonly WALK_TRANSITION_MS = 1000;
    private static readonly WALK_RETRY_MS = 500;
    private static readonly BASE_AMPLITUDE_HOLD_MS = 1500;

    private readonly hexapod: Hexapod;
    private readonly moves = new Map<string, { x: number, y: number }>();
    private readonly turns = new Map<string, number>();

    private engaged = false;
    private baseAmplitude = 30;
    private lastRelease = 0;
    private lastSend = 0;
    private lastWalkCommand = 0;
    private lastDirection: number | null = null;
    private lastAmplitude: number | null = null;
    private lastCommandedAmplitude: number | null = null;
    private lastRotation: number | null = null;

    constructor(hexapod: Hexapod) {
        this.hexapod = hexapod;
    }

    setMove(source: string, x: number, y: number) {
        this.moves.set(source, { x, y });
    }

    setTurn(source: string, value: number) {
        this.turns.set(source, value);
    }

    /** True when an input other than `exceptSource` is currently active. */
    hasInputOtherThan(exceptSource: string): boolean {
        for (const [source, move] of this.moves) {
            if (source !== exceptSource && DriveController.moveMagnitude(move) > 0) return true;
        }
        for (const [source, turn] of this.turns) {
            if (source !== exceptSource && DriveController.turnMagnitude(turn) > 0) return true;
        }
        return false;
    }

    /** Drop a source; ends the drive session right away when nothing else is active. */
    releaseSource(source: string, stopWalking: boolean) {
        this.moves.delete(source);
        this.turns.delete(source);
        if (this.engaged && !this.strongestMove() && !this.strongestTurn()) {
            this.finish(stopWalking);
        }
    }

    update(now: number) {
        const move = this.strongestMove();
        const turn = this.strongestTurn();

        if (!move && !turn) {
            if (this.engaged) this.finish(true);
            return;
        }
        if (now - this.lastSend < DriveController.SEND_INTERVAL_MS) return;
        this.lastSend = now;

        if (!this.engaged) {
            this.engaged = true;
            if (now - this.lastRelease > DriveController.BASE_AMPLITUDE_HOLD_MS
                && this.hexapod.hexapodStruct.amplitude !== this.lastCommandedAmplitude) {
                this.baseAmplitude = Math.max(DriveController.MIN_AMPLITUDE, this.hexapod.hexapodStruct.amplitude);
            }
        }

        // The gait ignores the direction while a rotation is set, so a turn wins over a move,
        // and the amplitude (not the rotation value) sets how fast the bot turns.
        let amplitude: number;
        let rotation: number;
        if (turn !== null) {
            amplitude = Math.max(DriveController.MIN_AMPLITUDE, Math.round(Math.abs(turn) * this.baseAmplitude));
            rotation = -Math.sign(turn) * Math.max(1, Math.round(Math.abs(turn) * DriveController.MAX_TURN_DEG));
        } else {
            const walk = move!;
            amplitude = Math.max(DriveController.MIN_AMPLITUDE, Math.round(walk.magnitude * this.baseAmplitude));
            rotation = 0;
            const angle = this.angleOf(walk.x, walk.y);
            if (angle !== this.lastDirection) {
                this.lastDirection = angle;
                this.hexapod.commitDirection(angle);
            }
        }

        if (rotation !== this.lastRotation) {
            this.lastRotation = rotation;
            this.hexapod.commitSignedRotation(rotation);
        }
        if (amplitude !== this.lastAmplitude) {
            this.lastAmplitude = amplitude;
            this.lastCommandedAmplitude = amplitude;
            this.hexapod.commitAmplitude(amplitude);
        }

        if (!this.hexapod.isMoving && now - this.lastWalkCommand > DriveController.WALK_RETRY_MS) {
            this.lastWalkCommand = now;
            this.hexapod.commitWalkStatus(true, DriveController.WALK_TRANSITION_MS);
        }
    }

    private finish(stopWalking: boolean) {
        this.engaged = false;
        this.lastRelease = performance.now();
        if (this.lastRotation !== 0) {
            this.hexapod.commitSignedRotation(0);
        }
        if (stopWalking) {
            this.hexapod.commitWalkStatus(false, DriveController.WALK_TRANSITION_MS);
        }
        this.lastCommandedAmplitude = this.baseAmplitude;
        this.hexapod.commitAmplitude(this.baseAmplitude);
        this.lastDirection = null;
        this.lastAmplitude = null;
        this.lastRotation = null;
    }

    private angleOf(x: number, y: number): number {
        const deg = (Math.atan2(-x, y) * 180) / Math.PI;
        const snapped = Math.round(deg / DriveController.DIRECTION_STEP_DEG) * DriveController.DIRECTION_STEP_DEG;
        return ((snapped % 360) + 360) % 360;
    }

    /** Strongest move across sources, with the dead zone removed and magnitude rescaled to [0, 1]. */
    private strongestMove(): { x: number, y: number, magnitude: number } | null {
        let best: { x: number, y: number, magnitude: number } | null = null;
        for (const move of this.moves.values()) {
            const magnitude = DriveController.moveMagnitude(move);
            if (magnitude > 0 && (!best || magnitude > best.magnitude)) {
                best = { x: move.x, y: move.y, magnitude };
            }
        }
        return best;
    }

    private strongestTurn(): number | null {
        let best: number | null = null;
        for (const turn of this.turns.values()) {
            if (DriveController.turnMagnitude(turn) > 0 && (best === null || Math.abs(turn) > Math.abs(best))) {
                best = turn;
            }
        }
        if (best === null) return null;
        const scaled = (Math.abs(best) - DriveController.DEADZONE) / (1 - DriveController.DEADZONE);
        return Math.sign(best) * Math.min(1, scaled);
    }

    private static moveMagnitude(move: { x: number, y: number }): number {
        const raw = Math.hypot(move.x, move.y);
        if (raw <= DriveController.DEADZONE) return 0;
        return Math.min(1, (raw - DriveController.DEADZONE) / (1 - DriveController.DEADZONE));
    }

    private static turnMagnitude(turn: number): number {
        return Math.abs(turn) > DriveController.DEADZONE ? Math.abs(turn) : 0;
    }
}

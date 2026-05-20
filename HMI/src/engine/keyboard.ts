import Hexapod from "../entity/hexapod.ts";

/**
 * Keyboard piloting for the hexapod.
 *
 *   W / ArrowUp     forward
 *   S / ArrowDown   backward
 *   A / ArrowLeft   strafe left
 *   D / ArrowRight  strafe right
 *   Q               yaw left  (CCW)
 *   E               yaw right (CW)
 *   R               release yaw (rotation = 0)
 *   Space           stop walking
 *   Enter           start walking
 *
 * Bindings are ignored while the focus is in an input/select/textarea so the
 * panel and modal forms keep working normally.
 */
export default class Keyboard {
    private static readonly DEFAULT_YAW_RATE_DEG = 30;

    private hexapod: Hexapod;

    constructor(hexapod: Hexapod) {
        this.hexapod = hexapod;
        document.addEventListener('keydown', (e) => this.onKeyDown(e));
    }

    private onKeyDown(event: KeyboardEvent) {
        if (this.isTypingInForm(event.target)) {
            return;
        }

        switch (event.code) {
            case 'KeyW':
            case 'ArrowUp':
                this.hexapod.commitDirection(0);
                this.hexapod.commitWalkStatus(true);
                break;
            case 'KeyS':
            case 'ArrowDown':
                this.hexapod.commitDirection(180);
                this.hexapod.commitWalkStatus(true);
                break;
            case 'KeyA':
            case 'ArrowLeft':
                this.hexapod.commitDirection(90);
                this.hexapod.commitWalkStatus(true);
                break;
            case 'KeyD':
            case 'ArrowRight':
                this.hexapod.commitDirection(270);
                this.hexapod.commitWalkStatus(true);
                break;
            case 'KeyQ':
                this.hexapod.commitSignedRotation(Keyboard.DEFAULT_YAW_RATE_DEG);
                break;
            case 'KeyE':
                this.hexapod.commitSignedRotation(-Keyboard.DEFAULT_YAW_RATE_DEG);
                break;
            case 'KeyR':
                this.hexapod.commitSignedRotation(0);
                break;
            case 'Space':
                event.preventDefault();
                this.hexapod.commitWalkStatus(false);
                break;
            case 'Enter':
                this.hexapod.commitWalkStatus(true);
                break;
            default:
                return;
        }
    }

    private isTypingInForm(target: EventTarget | null): boolean {
        if (!(target instanceof HTMLElement)) return false;
        const tag = target.tagName;
        return tag === 'INPUT' || tag === 'TEXTAREA' || tag === 'SELECT' || target.isContentEditable;
    }
}

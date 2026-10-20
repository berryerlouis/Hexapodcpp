import DriveController from "./driveController.ts";

/**
 * Browser gamepad piloting (standard mapping).
 *
 *   Left stick    translation (magnitude = stride amplitude)
 *   Right stick   yaw (X axis)
 *   B / Circle    stop
 */
export default class GamepadInput {
    private static readonly SOURCE = 'pad';
    private static readonly BUTTON_STOP = 1;

    private readonly drive: DriveController;
    private readonly onStop: () => void;
    private stopHeld = false;
    private wasConnected = false;

    constructor(drive: DriveController, onStop: () => void) {
        this.drive = drive;
        this.onStop = onStop;
    }

    update() {
        const pad = this.activePad();
        if (!pad) {
            if (this.wasConnected) {
                this.wasConnected = false;
                this.drive.setMove(GamepadInput.SOURCE, 0, 0);
                this.drive.setTurn(GamepadInput.SOURCE, 0);
            }
            return;
        }
        this.wasConnected = true;

        this.drive.setMove(GamepadInput.SOURCE, pad.axes[0] ?? 0, -(pad.axes[1] ?? 0));
        this.drive.setTurn(GamepadInput.SOURCE, pad.axes[2] ?? 0);

        const stopPressed = pad.buttons[GamepadInput.BUTTON_STOP]?.pressed ?? false;
        if (stopPressed && !this.stopHeld) {
            this.onStop();
        }
        this.stopHeld = stopPressed;
    }

    private activePad(): Gamepad | null {
        if (!navigator.getGamepads) return null;
        for (const pad of navigator.getGamepads()) {
            if (pad && pad.connected && pad.axes.length >= 3) return pad;
        }
        return null;
    }
}

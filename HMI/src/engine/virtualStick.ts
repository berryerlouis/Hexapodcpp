type StickAxes = 'xy' | 'x';

/**
 * On-screen stick driven by pointer events (mouse and touch). Reports x right-positive and
 * y up-positive in [-1, 1], and springs back to the centre on release.
 */
export default class VirtualStick {
    private readonly base: HTMLElement;
    private readonly knob: HTMLElement;
    private readonly axes: StickAxes;
    private readonly onChange: (x: number, y: number) => void;
    private pointerId: number | null = null;

    constructor(base: HTMLElement, axes: StickAxes, onChange: (x: number, y: number) => void) {
        this.base = base;
        this.knob = base.querySelector('.stick-knob') as HTMLElement;
        this.axes = axes;
        this.onChange = onChange;

        base.addEventListener('pointerdown', (e) => this.onDown(e));
        base.addEventListener('pointermove', (e) => this.onMove(e));
        base.addEventListener('pointerup', (e) => this.onUp(e));
        base.addEventListener('pointercancel', (e) => this.onUp(e));
        window.addEventListener('blur', () => this.reset());
    }

    reset() {
        this.pointerId = null;
        this.base.classList.remove('is-active');
        this.knob.style.transform = '';
        this.onChange(0, 0);
    }

    private onDown(event: PointerEvent) {
        if (this.pointerId !== null) return;
        event.preventDefault();
        this.pointerId = event.pointerId;
        this.base.setPointerCapture(event.pointerId);
        this.base.classList.add('is-active');
        this.track(event);
    }

    private onMove(event: PointerEvent) {
        if (event.pointerId !== this.pointerId) return;
        this.track(event);
    }

    private onUp(event: PointerEvent) {
        if (event.pointerId !== this.pointerId) return;
        this.reset();
    }

    private track(event: PointerEvent) {
        const rect = this.base.getBoundingClientRect();
        const travel = (rect.width - this.knob.offsetWidth) / 2;
        if (travel <= 0) return;

        let dx = event.clientX - (rect.left + rect.width / 2);
        let dy = this.axes === 'x' ? 0 : event.clientY - (rect.top + rect.height / 2);
        const length = Math.hypot(dx, dy);
        if (length > travel) {
            dx = (dx / length) * travel;
            dy = (dy / length) * travel;
        }
        this.knob.style.transform = `translate(${dx}px, ${dy}px)`;
        this.onChange(dx / travel, -dy / travel);
    }
}

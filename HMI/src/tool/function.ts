export function getCanvas(): HTMLCanvasElement {
   // const app = document.querySelector<HTMLDivElement>('#app')!;
    const canvas: HTMLCanvasElement = document.getElementsByTagName('canvas')[0]
    canvas.width = innerWidth
    canvas.height = innerHeight
    return canvas
}
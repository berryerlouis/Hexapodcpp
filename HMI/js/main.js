
import Console from './ui/Console.js'
import Canvas from './ui/canvas.js'
import Command from './ui/Command.js'
import Communication from './ui/Communication.js';


const uiCanvas = new Canvas(window.innerWidth, window.innerHeight - document.body.children[0].children[0].clientHeight);
const uiConsole = new Console();
const communication = new Communication(uiConsole,uiCanvas);
const menu = new Command(communication);
function animate() {
    requestAnimationFrame(animate);
    uiCanvas.animate();
}

animate();
import Graphic  from './engine/graphic.ts'
import Camera  from './engine/camera.ts'
import Light  from './engine/light.ts'
import Control from "./engine/control.ts";
import Hexapod from "./entity/hexapod.ts";
import World from "./entity/world.ts";
import Display from "./engine/scene.ts";
import Panel from "./engine/panel.ts";
import Socket from "./communication/socket.ts";
import DirectionArrow from "./engine/directionArrow.ts";


export default class Ui {

    scene = new Display();
    camera = new Camera();
    world = new World(10,10);
    light = new Light()
    graphic = new Graphic(this.scene, this.camera);
    control = new Control(this.camera, this.graphic);
    socket:Socket;
    hexapod:Hexapod;
    panel:Panel;
    directionArrow:DirectionArrow;
    constructor(socket:Socket) {
        this.socket = socket;
        this.hexapod = new Hexapod(socket);
        this.directionArrow = new DirectionArrow(socket);
        this.panel = new Panel(document.getElementById('panel')!, this.hexapod, socket);

        this.scene.add( this.world );
        this.scene.add( this.light );
        this.scene.add( this.hexapod );
        this.graphic.onUpdate(dt => {
            this.control.update(dt);
            this.world.update();
            this.hexapod.update();
            this.panel.update();
            this.camera.update();
            this.light.update(this.hexapod);
        });
    }

    update() {

    }
}



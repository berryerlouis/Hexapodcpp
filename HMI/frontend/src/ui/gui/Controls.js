import {GUI} from "../../../vendor/dat.gui/build/dat.gui.module.js";
import {Pane} from 'tweakpane';
import ClusterCommand from "./ClusterCommand.js";

export default class Controls {

    constructor(messageManager, robot) {

        this.robot = robot;
        const gui = new Pane();
        this.gui.domElement.id = 'gui';
        new ClusterCommand(this, messageManager, robot);
    }

    updateDisplay() {
        this.gui.updateDisplay();
    }

    addFolder(gui, folderName) {
        let folder = this.folderExists(gui, folderName);
        if (!folder) {
            folder = gui.addFolder(folderName)
        }
        return folder;
    }

    getFolder(gui, folderName) {
        return gui.folderExists(gui, folderName);
    }

    folderExists(gui, folderName) {
        return gui.__folders && gui.__folders[folderName];
    }


}
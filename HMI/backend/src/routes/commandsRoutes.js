"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var express_1 = require("express");
var commandsController_1 = require("../controllers/commandsController");
var router = (0, express_1.Router)();
router.get('/connect', commandsController_1.connect);
exports.default = router;

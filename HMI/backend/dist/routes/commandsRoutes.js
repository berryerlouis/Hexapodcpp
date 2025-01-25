"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = require("express");
const commandsController_1 = require("../controllers/commandsController");
const router = (0, express_1.Router)();
router.get('/connect', commandsController_1.connect);
exports.default = router;

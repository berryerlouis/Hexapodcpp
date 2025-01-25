"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
require('dotenv').config({ path: __dirname + '/.env' });
const express_1 = __importDefault(require("express"));
const path_1 = __importDefault(require("path"));
const commandsRoutes_1 = __importDefault(require("./routes/commandsRoutes"));
const tcpclient_1 = __importDefault(require("./tcpclient"));
const websocket_1 = require("./websocket");
(0, websocket_1.connect)(new tcpclient_1.default);
// express app
const app = (0, express_1.default)();
const PORT = process.env.SERVER_PORT || '3000';
app.use(express_1.default.static(path_1.default.join(__dirname, '../../frontend/public')));
app.use(express_1.default.json());
app.use('/api/commands', commandsRoutes_1.default);
app.listen(PORT, () => {
    console.log(`Server is running on port http://localhost:${PORT}`);
});

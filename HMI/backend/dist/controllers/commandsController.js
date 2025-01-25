"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.connect = void 0;
const tcpclient_1 = __importDefault(require("../tcpclient"));
const connect = (req, res) => {
    tcpclient_1.default.write("<000000>");
    res.send('Message sent to hexapod');
};
exports.connect = connect;

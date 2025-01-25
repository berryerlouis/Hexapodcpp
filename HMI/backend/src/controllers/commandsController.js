"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.connect = void 0;
var tcpclient_1 = require("../tcpclient");
var connect = function (req, res) {
    tcpclient_1.default.write("<000000>");
    res.send('Message sent to hexapod');
};
exports.connect = connect;

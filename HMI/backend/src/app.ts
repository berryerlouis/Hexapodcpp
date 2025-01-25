
require('dotenv').config({ path: __dirname+'/.env' });
import express from 'express';
import path from 'path';
import commandsRoutes from './routes/commandsRoutes';
import HexapodSocket from './tcpclient';
import {connect} from './websocket';

connect(new HexapodSocket);
// express app
const app = express();

const PORT = process.env.SERVER_PORT || '3000';
app.use(express.static(path.join(__dirname, '../../frontend/public')));

app.use(express.json());
app.use('/api/commands', commandsRoutes);

app.listen(PORT, () => {
    console.log(`Server is running on port http://localhost:${PORT}`);
});

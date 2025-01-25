import { Request, Response } from 'express';
import hexapodSocket from '../tcpclient';

export const connect = (req: Request, res: Response) => {
  hexapodSocket.write("<000000>");
  res.send('Message sent to hexapod');
};
import { Router } from 'express';
import { connect } from '../controllers/commandsController';

const router = Router();

router.get('/connect', connect);

export default router;
#include "Frame.h"

namespace Cluster {
Frame::Frame(void)
{
	Reset();
}

Core::CoreStatus Frame::Build (uint8_t clusterId, uint8_t commandId, uint8_t *params, uint8_t nbParams)
{
	if (nbParams > FRAME_MAX_PARAMS)
	{
		return (Core::CoreStatus::CORE_ERROR);
	}
	this->clusterId = clusterId;
	this->commandId = commandId;
	this->nbParams  = nbParams;

	memset( (void *) this->params, 0U, FRAME_MAX_PARAMS);
	memcpy( (void *) this->params, params, nbParams);

	return (Core::CoreStatus::CORE_OK);
}

void Frame::Reset (void)
{
	this->clusterId = 0U;
	this->commandId = 0U;
	this->nbParams  = 0U;
	memset( (void *) this->params, 0U, FRAME_MAX_PARAMS);
}
}

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace Cluster {
#define FRAME_MAX_PARAMS    20U
struct Frame
{
public:
	Frame(void);
	~Frame(void) = default;

	void Reset(void);
	bool Build(uint8_t clusterId, uint8_t commandId, uint8_t *params, uint8_t nbParams);

	uint8_t clusterId;
	uint8_t commandId;
	uint8_t nbParams;
	uint8_t params[FRAME_MAX_PARAMS];

private:
};
}

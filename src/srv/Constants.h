#pragma once

#include <stdint.h>

enum EServices
{
	BATTERY      = 0U,
	CONTROL      = 1U,
	DISPLAY      = 2U,
	GENERAL      = 3U,
	ORIUENTATION = 4U,
	PROXIMITY    = 5U
};

static const uint8_t NB_SERVICES = 6U;

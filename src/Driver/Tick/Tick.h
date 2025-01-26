#pragma once

#ifdef RPI
#include "TickRPI.h"
#elif AVR
#include "TickAVR.h"
#elif X64
#include "TickX64.h"
#endif

#pragma once

#ifdef RPI
#include "AdcX64.h"
#elif AVR
#include "AdcAVR.h"
#elif X64
#include "AdcX64.h"
#endif

#pragma once

#ifdef RPI
#include "AdcRPI.h"
#elif AVR
#include "AdcAVR.h"
#elif X64
#include "AdcX64.h"
#endif

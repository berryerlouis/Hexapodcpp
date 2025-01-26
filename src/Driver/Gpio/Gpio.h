#pragma once

#ifdef RPI
#include "GpioX64.h"
#elif AVR
#include "GpioAVR.h"
#elif X64
#include "GpioX64.h"
#endif


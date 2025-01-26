#pragma once

#ifdef RPI
#include "UartRPI.h"
#elif AVR
#include "UartAVR.h"
#elif X64
#include "UartX64.h"
#endif

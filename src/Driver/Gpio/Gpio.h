#pragma once

#ifdef RPI
#include "GpioRPI.h"
#elif X64
#include "GpioX64.h"
#endif

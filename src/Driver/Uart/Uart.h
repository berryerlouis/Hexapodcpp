#pragma once

#ifdef RPI
#include "UartRPI.h"
#elif X64
#include "UartX64.h"
#endif
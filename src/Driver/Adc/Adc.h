#pragma once

#ifdef RPI
#include "AdcRPI.h"
#elif X64
#include "AdcX64.h"
#endif

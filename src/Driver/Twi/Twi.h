#pragma once

#ifdef RPI
#include "TwiRPI.h"
#elif AVR
#include "TwiAVR.h"
#elif X64
#include "TwiX64.h"
#endif

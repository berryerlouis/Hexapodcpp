#pragma once
#pragma once

#ifdef RPI
#include "InputCaptureRPI.h"
#elif AVR
#include "InputCaptureAVR.h"
#elif X64
#include "InputCaptureX64.h"
#endif

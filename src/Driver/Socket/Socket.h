#pragma once

#ifdef RPI
#include "SocketRPI.h"
#elif AVR
#include "SocketAVR.h"
#elif X64
#include "SocketX64.h"
#endif

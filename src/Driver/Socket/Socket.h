#pragma once

#ifdef RPI
#include "SocketRPI.h"
#elif X64
#include "SocketX64.h"
#endif

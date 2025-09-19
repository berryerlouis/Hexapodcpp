#pragma once

#include <string>
#include "../Core/Logger.h"

namespace Move
{
#define LOG_MOVE_DEBUG(cmp, ...) LOG_GENERIC(DEBUG, MOVE, "[" cmp "] " __VA_ARGS__)
#define LOG_MOVE_INFO(cmp, ...)  LOG_GENERIC(INFO, MOVE,  "[" cmp "] "  __VA_ARGS__)
#define LOG_MOVE_WARNING(cmp, ...) LOG_GENERIC(WARNING, MOVE, "[" cmp "] "  __VA_ARGS__)
#define LOG_MOVE_ERROR(cmp, ...) LOG_GENERIC(ERROR, MOVE, "[" cmp "] "  __VA_ARGS__)

}

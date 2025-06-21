#pragma once
#include "../Core/CoreInterface.h"

namespace Driver
{
#define LOG_DRIVER_DEBUG(cmp, ...) LOG_GENERIC(DEBUG, DRIVER, "[" cmp "] " __VA_ARGS__)
#define LOG_DRIVER_INFO(cmp, ...)  LOG_GENERIC(INFO, DRIVER,  "[" cmp "] "  __VA_ARGS__)
#define LOG_DRIVER_WARNING(cmp, ...) LOG_GENERIC(WARNING, DRIVER, "[" cmp "] "  __VA_ARGS__)
#define LOG_DRIVER_ERROR(cmp, ...) LOG_GENERIC(ERROR, DRIVER, "[" cmp "] "  __VA_ARGS__)

    class DriverInterface : public Core::CoreInterface {
    public:
        DriverInterface() = default;

        ~DriverInterface() = default;

        virtual Core::Status Initialize(void) = 0;

        virtual void Update(const uint64_t currentTime) = 0;
    };
}

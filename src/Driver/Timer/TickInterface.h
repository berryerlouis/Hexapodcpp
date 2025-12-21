#pragma once
#include <stdint.h>

namespace Driver
{
    namespace Timer
    {
        class TickInterface {
        public:
            TickInterface() = default;

            ~TickInterface() = default;

            virtual uint64_t GetUs(void) = 0;

            virtual uint64_t GetMs(void) = 0;

            virtual void     DelayMs(const uint64_t delayMs) = 0;

            virtual void     DelayUs(const uint64_t delayUs) = 0;
        };
    } // namespace Timer
} // namespace Driver

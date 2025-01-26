#pragma once

#include "TickInterface.h"
#include <chrono>

namespace Driver
{
    namespace Tick
    {
        class Tick : public TickInterface {
        public:
            Tick();

            ~Tick() = default;

            virtual uint64_t GetUs(void) final override;

            virtual uint64_t
            GetMs(void) final override;

            virtual void DelayMs(uint64_t delayMs) final override;

            virtual void DelayUs(uint64_t delayUs) final override;

            std::chrono::time_point<std::chrono::system_clock> now;
        };
    } // namespace Tick
} // namespace Driver

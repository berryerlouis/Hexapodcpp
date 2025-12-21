#pragma once

#include <chrono>
#include "TickInterface.h"

namespace Driver
{
    namespace Timer
    {
        class Tick : public TickInterface {
        public:
            ~Tick() = default;

            virtual uint64_t                                          GetUs(void) final override;

            virtual uint64_t                                          GetMs(void) final override;

            virtual void                                              DelayMs(uint64_t delayMs) final override;

            virtual void                                              DelayUs(uint64_t delayUs) final override;

            static std::chrono::time_point<std::chrono::system_clock> now;

            static Tick                                              &GetInstance();

        private:
            Tick();

            Tick(const Tick &) = delete;

            Tick &operator=(const Tick &) = delete;
        };
    } // namespace Timer
} // namespace Driver

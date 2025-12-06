#pragma once

#include "TickInterface.h"
#include <chrono>

namespace Driver
{
    namespace Timer
    {
        class Tick : public TickInterface {
        public:
            virtual uint64_t GetUs(void) final override;

            virtual uint64_t
            GetMs(void) final override;

            virtual void DelayMs(uint64_t delayMs) final override;

            virtual void DelayUs(uint64_t delayUs) final override;

            std::chrono::time_point<std::chrono::system_clock> now;

            static Tick &GetInstance();

#ifdef GTEST
            void
            SetUs(const uint64_t fakeUs) {
                this->mFakeUs = fakeUs;
            }

            void
            ResetFake() {
                this->mFakeUs = 0U;
            }
#endif

        private:
            Tick();

            Tick(const Tick &) = delete;

            Tick &operator=(const Tick &) = delete;

#ifdef GTEST
            uint64_t mFakeUs = 0U;
            uint64_t mFakeMs = 0U;
#endif
        };
    } // namespace Tick
} // namespace Driver
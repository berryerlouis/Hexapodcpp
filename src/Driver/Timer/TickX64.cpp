#include "TickX64.h"

#include <chrono>
#include <thread>

namespace Driver
{
    namespace Timer
    {
        auto Tick::GetInstance() -> Tick & {
            static Tick instance;
            return instance;
        }

        Tick::Tick() {
            this->mStart = clock::now();
        }

        auto Tick::GetUs() -> uint64_t {
#ifdef GTEST
            return mFakeUs;
#else
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                    clock::now() - this->mStart);
            return static_cast<uint64_t>(duration.count());
#endif
        }

        auto Tick::GetMs() -> uint64_t {
#ifdef GTEST
            return mFakeMs;
#else
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    clock::now() - this->mStart);
            return static_cast<uint64_t>(duration.count());
#endif
        }

        void Tick::DelayMs(const uint64_t delayMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        void Tick::DelayUs(const uint64_t delayUs) {
            std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
        }
    } // namespace Timer
} // namespace Driver

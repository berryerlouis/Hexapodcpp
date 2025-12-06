#include <thread>
#include "Tick.h"

namespace Driver
{
    namespace Timer
    {
        uint64_t start;
        std::chrono::time_point<std::chrono::system_clock> now;

        Tick &Tick::GetInstance() {
            static Tick instance;
            return instance;
        }

        Tick::Tick(void) {
            now = std::chrono::system_clock::now();
            start = GetMs();
        }

        uint64_t Tick::GetUs(void) {
#ifdef GTEST
            return mFakeUs;
#else
            const auto duration = now.time_since_epoch();
            now = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::microseconds>(duration).count() - start;
#endif
        }

        uint64_t Tick::GetMs(void) {
#ifdef GTEST
            return mFakeMs;
#else
            const auto duration = now.time_since_epoch();
            now = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() - start;
#endif
        }

        void Tick::DelayMs(const uint64_t delayMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        void Tick::DelayUs(const uint64_t delayUs) {
            std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
        }
    }
}
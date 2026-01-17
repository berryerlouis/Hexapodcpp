#include <chrono>
#include <thread>
#include "Tick.h"

namespace Driver
{
    namespace Timer
    {
        uint64_t                                           start;
        std::chrono::time_point<std::chrono::steady_clock> start_time;

        Tick                                              &Tick::GetInstance() {
            static Tick instance;
            return instance;
        }

        Tick::Tick(void) {
            start_time = std::chrono::steady_clock::now();
            start = GetMs();
        }

        uint64_t Tick::GetUs(void) {
            const auto duration = std::chrono::steady_clock::now() - start_time;
            return std::chrono::duration_cast<std::chrono::microseconds>(
                           duration)
                    .count();
        }

        uint64_t Tick::GetMs(void) {
            const auto duration = std::chrono::steady_clock::now() - start_time;
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                           duration)
                    .count();
        }

        void Tick::DelayMs(const uint64_t delayMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        void Tick::DelayUs(const uint64_t delayUs) {
            std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
        }
    } // namespace Timer
} // namespace Driver

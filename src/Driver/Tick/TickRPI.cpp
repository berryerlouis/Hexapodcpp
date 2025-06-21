#include "Tick.h"
#include <chrono>
#include <thread>

namespace Driver
{
    namespace Tick
    {
        uint64_t start;
        std::chrono::time_point<std::chrono::steady_clock> start_time;

        Tick::Tick(void) {
            start_time = std::chrono::steady_clock::now();
            start = GetMs();
            LOG_DRIVER_DEBUG("Tick", "Initialized.");
        }

        uint64_t Tick::GetUs(void) {
            const auto now = std::chrono::steady_clock::now();
            const auto duration = now - start_time;
            return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        }

        uint64_t Tick::GetMs(void) {
            const auto now = std::chrono::steady_clock::now();
            const auto duration = now - start_time;
            return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        }

        void Tick::DelayMs(const uint64_t delayMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        void Tick::DelayUs(const uint64_t delayUs) {
            std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
        }
    }
}

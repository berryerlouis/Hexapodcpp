#include "Tick.h"
#include <chrono>


namespace Driver
{
    namespace Tick
    {
        std::chrono::time_point<std::chrono::system_clock> now;

        Tick::Tick(void) {
            now = std::chrono::system_clock::now();
        }

        uint64_t Tick::GetUs(void) { return 0; }

        uint64_t Tick::GetMs(void) {
            auto duration = now.time_since_epoch();
            return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        }

        void Tick::DelayMs(uint64_t delayMs) {
            (void) delayMs;
        }

        void Tick::DelayUs(uint64_t delayUs) {
            (void) delayUs;
        }
    }
}

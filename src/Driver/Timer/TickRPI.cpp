#include "TickRPI.h"
#include <chrono>
#include <thread>

namespace Driver
{
    namespace Timer
    {
        Tick &Tick::GetInstance() {
            static Tick instance;
            return instance;
        }

        Tick::Tick() {
            this->mStart = clock::now();
        }

        uint64_t Tick::GetUs() {
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                    clock::now() - this->mStart);
            return static_cast<uint64_t>(duration.count());
        }

        uint64_t Tick::GetMs() {
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    clock::now() - this->mStart);
            return static_cast<uint64_t>(duration.count());
        }

        void Tick::DelayMs(const uint64_t delayMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        void Tick::DelayUs(const uint64_t delayUs) {
            std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
        }
    } // namespace Timer
} // namespace Driver

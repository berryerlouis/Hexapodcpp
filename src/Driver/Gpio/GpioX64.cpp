#include "Gpio.h"

namespace Driver
{
    namespace Gpio
    {
        Gpio::Gpio(const SGpio &gpio, const EPortDirection &portDirection)
            : mGpio(gpio) {
            (void) portDirection;
        }

        SGpio &Gpio::GetPin() {
            return (this->mGpio);
        }

        Core::Status Gpio::Set() {
            return (Core::Status::CORE_OK);
        }

        Core::Status Gpio::Reset() {
            return (Core::Status::CORE_OK);
        }

        Core::Status Gpio::Pwm(const uint16_t delay) {
            (void) delay;
            return (Core::Status::CORE_OK);
        }

        bool Gpio::Get() {
            return (false);
        }

        void Gpio::SetInterruptPin() {
        }

        void Gpio::SetInterruptPin(const InterruptCallback callback) {
            (void) callback;
        }

        void Gpio::ResetInterruptPin() {
        }
    } // namespace Gpio
} // namespace Driver

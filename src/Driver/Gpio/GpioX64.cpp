#include "Gpio.h"

namespace Driver
{
    namespace Gpio
    {
        Gpio::Gpio(const SGpio &gpio, const EPortDirection &portDirection) :
            mGpio(gpio) {
            (void) portDirection;
            LOG_DRIVER_DEBUG("Gpio", "pin %d Initialized.", this->mGpio.pin);
        }

        SGpio &Gpio::GetPin(void) {
            return (this->mGpio);
        }

        Core::Status Gpio::Set(void) {
            return (Core::Status::CORE_OK);
        }

        Core::Status Gpio::Reset(void) {
            return (Core::Status::CORE_OK);
        }

        bool Gpio::Get(void) {
            return (false);
        }

        void Gpio::SetInterruptPin(void) {
        }

        void Gpio::SetInterruptPin(const InterruptCallback callback) {
            (void) callback;
        }

        void Gpio::ResetInterruptPin(void) {
        }
    } // namespace Gpio
} // namespace Driver

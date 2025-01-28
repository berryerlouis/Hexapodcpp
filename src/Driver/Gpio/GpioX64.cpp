#include "Gpio.h"

namespace Driver
{
    namespace Gpio
    {
        static bool once = false;

        Gpio::Gpio(const SGpio &gpio, const EPortDirection &portDirection) :
            mGpio(gpio) {
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

        void Gpio::SetInterruptPin(void (*function)(void)) {
        }

        void Gpio::ResetInterruptPin(void) {
        }
    } // namespace Gpio
} // namespace Driver

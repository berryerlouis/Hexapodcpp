#include "Gpio.h"

namespace Driver
{
    namespace Gpio
    {
        Gpio::GpioRegister Gpio::sGpioRegisters[] = {};

        Gpio::Gpio(const SGpio &gpio, const EPortDirection &portDirection) : mGpio(gpio),
                                                                             mPortDirection(portDirection),
                                                                             mGpioRegister() {
        }

        EPin &Gpio::GetPin(void) {
            return (this->mGpio.pin);
        }

        Core::CoreStatus Gpio::Set(void) {
            return (Core::CoreStatus::CORE_OK);
        }

        Core::CoreStatus Gpio::Reset(void) {
            return (Core::CoreStatus::CORE_OK);
        }

        bool Gpio::Get(void) {
            return (0U);
        }

        void Gpio::SetInterruptPin(void) {
        }

        void Gpio::ResetInterruptPin(void) {
        }
    }
}

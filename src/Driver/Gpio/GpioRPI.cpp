#include "Gpio.h"
#include "wiringPi/wiringPi.h"

namespace Driver
{
    namespace Gpio
    {
        static bool once = false;

        Gpio::Gpio(const SGpio &gpio, const EPortDirection &portDirection) :
            mGpio(gpio) {
            if (!once) {
                wiringPiSetupGpio();
                once = true;
            }
            pinMode(this->mGpio.pin, portDirection == EPortDirection::IN ? INPUT : OUTPUT);
        }

        SGpio &Gpio::GetPin(void) {
            return (this->mGpio);
        }

        Core::Status Gpio::Set(void) {
            digitalWrite(this->mGpio.pin, HIGH);
            return (Core::Status::CORE_OK);
        }

        Core::Status Gpio::Reset(void) {
            digitalWrite(this->mGpio.pin, LOW);
            return (Core::Status::CORE_OK);
        }

        bool Gpio::Get(void) {
            return digitalRead(this->mGpio.pin);
        }

        void Gpio::SetInterruptPin(void) {
        }

        void Gpio::ResetInterruptPin(void) {
        }
    } // namespace Gpio
} // namespace Driver

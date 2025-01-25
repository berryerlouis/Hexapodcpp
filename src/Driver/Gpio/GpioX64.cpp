#include "GpioX64.h"
#ifdef RPI
#include "wiringPi/wiringPi.h"
#endif

namespace Driver
{
    namespace Gpio
    {
        static bool once = false;

        Gpio::Gpio(const SGpio &gpio, const EPortDirection &portDirection) :
            mGpio(gpio) {
#ifdef RPI
            if (!once) {
                wiringPiSetupGpio();
                once = true;
            }
            pinMode(this->mGpio.pin, portDirection == EPortDirection::IN ? INPUT : OUTPUT);
#endif
        }

        SGpio &Gpio::GetPin(void) { return (this->mGpio); }

        Core::Status Gpio::Set(void) {
#ifdef RPI
            digitalWrite(this->mGpio.pin, HIGH);
#endif
            return (Core::Status::CORE_OK);
        }

        Core::Status Gpio::Reset(void) {
#ifdef RPI
            digitalWrite(this->mGpio.pin, LOW);
            return (Core::Status::CORE_OK);
#endif
            return (Core::Status::CORE_OK);
        }

        bool Gpio::Get(void) {
#ifdef RPI
            return digitalRead(this->mGpio.pin);
#endif
            return (false);
        }

        void Gpio::SetInterruptPin(void) {
        }

        void Gpio::ResetInterruptPin(void) {
        }
    } // namespace Gpio
} // namespace Driver

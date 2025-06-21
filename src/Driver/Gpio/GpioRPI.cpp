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
            if (portDirection == EPortDirection::IN) {
                pullUpDnControl(this->mGpio.pin, PUD_DOWN);
            }
            LOG_DRIVER_DEBUG("Gpio", "pin %d Initialized.", this->mGpio.pin);
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

        void Gpio::SetInterruptPin(const InterruptCallback callback) {
            wiringPiISR(this->mGpio.pin, INT_EDGE_BOTH, callback);
            (void) callback;
        }

        void Gpio::ResetInterruptPin(void) {
            wiringPiISRStop(this->mGpio.pin);
        }
    } // namespace Gpio
} // namespace Driver

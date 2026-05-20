#include <wiringPi/wiringPi.h>
#include "Gpio.h"

namespace Driver
{
    namespace Gpio
    {
        static bool once = false;

        Gpio::Gpio(const SGpio &gpio, const EPortDirection &portDirection)
            : mGpio(gpio) {
            if (!once) {
                wiringPiSetupGpio();
                once = true;
            }
            if ((portDirection == EPortDirection::IN) || (portDirection == EPortDirection::OUT)) {
                pinMode(this->mGpio.pin, portDirection == EPortDirection::IN ? INPUT : OUTPUT);
            } else {
                pinMode(this->mGpio.pin, PWM_OUTPUT);
            }
            if (portDirection == EPortDirection::IN) {
                pullUpDnControl(this->mGpio.pin, PUD_DOWN);
            }
        }

        SGpio &Gpio::GetPin() {
            return this->mGpio;
        }

        Core::Status Gpio::Set() {
            digitalWrite(this->mGpio.pin, HIGH);
            return Core::Status::CORE_OK;
        }

        Core::Status Gpio::Reset() {
            digitalWrite(this->mGpio.pin, LOW);
            return Core::Status::CORE_OK;
        }

        Core::Status Gpio::Pwm(const uint16_t delay) {
            pwmWrite(this->mGpio.pin, delay);
            return Core::Status::CORE_OK;
        }

        bool Gpio::Get() {
            return digitalRead(this->mGpio.pin);
        }

        void Gpio::SetInterruptPin() {
        }

        void Gpio::SetInterruptPin(const InterruptCallback callback) {
            wiringPiISR(this->mGpio.pin, INT_EDGE_BOTH, callback);
            (void) callback;
        }

        void Gpio::ResetInterruptPin() {
            wiringPiISRStop(this->mGpio.pin);
        }
    } // namespace Gpio
} // namespace Driver

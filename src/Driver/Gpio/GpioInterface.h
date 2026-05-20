#pragma once

#include <stdint.h>
#include "../../Core/Status.h"
#include "../DriverInterface.h"

namespace Driver
{
    namespace Gpio
    {
        enum EPortDirection {
            IN = 0U,
            OUT = 1U,
            PWM = 2U,
        };

        enum EPort {
            PORT_A,
            PORT_B,
            PORT_C,
            PORT_D
        };

#define NB_PORT 4U

        struct SGpio {
            uint8_t pin;
        };

        class GpioInterface {
        public:
            typedef void (*InterruptCallback)(void);

            GpioInterface() = default;

            virtual Core::Status Set(void) = 0;

            virtual Core::Status Reset(void) = 0;

            virtual Core::Status Pwm(const uint16_t delay) = 0;

            virtual SGpio       &GetPin(void) = 0;

            virtual bool         Get(void) = 0;

            virtual void         SetInterruptPin(void) = 0;

            virtual void         SetInterruptPin(const InterruptCallback callback) = 0;

            virtual void         ResetInterruptPin(void) = 0;
        };
    } // namespace Gpio
} // namespace Driver

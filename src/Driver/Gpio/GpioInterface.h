#pragma once

#include <stdint.h>
#include "../../Core/Status.h"

namespace Driver
{
    namespace Gpio
    {
        enum EPortDirection {
            IN = 0U,
            OUT = 1U
        };

        enum EPort {
            PORT_A,
            PORT_B,
            PORT_C,
            PORT_D
        };

#define NB_PORT    4U

        enum EPin {
            PIN_0,
            PIN_1,
            PIN_2,
            PIN_3,
            PIN_4,
            PIN_5,
            PIN_6,
            PIN_7,
        };

        struct SGpio {
            EPort port;
            EPin pin;
        };


        class GpioInterface {
        public:
            GpioInterface() = default;

            ~GpioInterface() = default;

            virtual Core::CoreStatus Set(void) = 0;

            virtual Core::CoreStatus Reset(void) = 0;

            virtual EPin &GetPin(void) = 0;

            virtual bool Get(void) = 0;

            virtual void SetInterruptPin(void) = 0;

            virtual void ResetInterruptPin(void) = 0;
        };
    }
}

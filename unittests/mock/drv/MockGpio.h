#pragma once
#include <gmock/gmock.h>
#include "../../../src/Driver/Gpio/GpioInterface.h"

namespace Driver
{
    namespace Gpio
    {
        class MockGpio : public GpioInterface {
        public:
            MOCK_METHOD0(Set, Core::Status(void));

            MOCK_METHOD0(Reset, Core::Status(void));

            MOCK_METHOD1(Pwm, Core::Status(const uint16_t));

            MOCK_METHOD0(Get, bool(void));

            MOCK_METHOD0(GetPin, SGpio &(void) );

            MOCK_METHOD0(SetInterruptPin, void(void));

            MOCK_METHOD1(SetInterruptPin, void(void (*)(void)));

            MOCK_METHOD0(ResetInterruptPin, void(void));
        };
    } // namespace Gpio
} // namespace Driver

#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/LedPwm/LedPwmInterface.h"

namespace Component
{
    namespace LedPwm
    {
        class MockLedPwm : public LedPwmInterface {
        public:
            MOCK_METHOD0(Initialize, Core::Status(void));

            MOCK_METHOD1(Update, void(const uint64_t));

            MOCK_METHOD0(Stop, void(void));

            MOCK_METHOD1(UpdateFrequency, void(const float));
        };
    } // namespace LedPwm
} // namespace Component

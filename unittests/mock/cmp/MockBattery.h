#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Battery/BatteryInterface.h"

namespace Component
{
    namespace Battery
    {
        class MockBattery : public BatteryInterface {
        public:
            MOCK_METHOD0(Initialize, Core::Status (void));

            MOCK_METHOD1(Update, void (const uint64_t));

            MOCK_METHOD0(GetState, BatteryState (void));

            MOCK_METHOD0(GetVoltage, uint16_t (void));

            MOCK_METHOD0(GetIntensity, uint16_t (void));
        };
    }
}
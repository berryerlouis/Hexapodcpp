#pragma once

#include "BatteryInterface.h"
#include "../Adc/Ads1115.h"

namespace Component
{
    namespace Battery
    {
        using namespace Driver;

        class Battery : public BatteryInterface {
        public:
            Battery(Adc::Ads1115Interface &adc);

            ~Battery() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual BatteryState GetState(void) final override;

            virtual uint16_t GetVoltage(void) final override;

            virtual uint16_t GetIntensity(void) final override;

        private:
            uint16_t mVoltage;
            uint16_t mIntensity;
            BatteryState mState;
            Adc::Ads1115Interface &mAdc;
        };
    }
}
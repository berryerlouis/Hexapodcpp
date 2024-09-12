#pragma once

#include "../../Driver/Adc/AdcInterface.h"
#include "BatteryInterface.h"
#include "BatteryObservable.h"

namespace Component
{
    namespace Battery
    {
        using namespace Driver;

        class Battery : public BatteryInterface {
        public:
            Battery(Adc::AdcInterface &adc);

            ~Battery() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual BatteryState GetState(void) final override;

            virtual uint16_t GetVoltage(void) final override;

            virtual Core::CoreStatus Attach(BatteryObserverInterface *observer) final override;

            virtual void Notify(const BatteryState &object) final override;

        private:
            uint16_t mVoltage;
            BatteryState mState;
            Adc::AdcInterface &mAdc;
            BatteryObservable mObservable;
        };
    }
}

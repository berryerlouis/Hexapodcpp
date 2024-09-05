#pragma once

#include "../../Component/Battery/BatteryInterface.h"
#include "../../Component/Battery/BatteryObserverInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Battery
    {
        using namespace Component::Battery;

        class ServiceBattery : public Service, public BatteryObserverInterface {
        public:
            ServiceBattery(BatteryInterface &batteryInterface);

            ~ServiceBattery() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void UpdatedBatteryState(const BatteryState &batteryState) final override;

        protected:
            BatteryInterface &mBatteryInterface;
        };
    }
}

#pragma once

#include "../Service.h"
#include "../../Component/Battery/BatteryInterface.h"
#include "../../Core/ObserverInterface.h"

namespace Service
{
    namespace Battery
    {
        using namespace Component;
        using namespace Component::Battery;

        class ServiceBattery : public Service, Core::ObserverInterface<BatteryStruct> {
        public:
            ServiceBattery(BatteryInterface &batteryInterface,
                           Event::MessageInterface &messageListener);

            ~ServiceBattery() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Notified(const BatteryStruct &battery) final override;

        protected:
            BatteryInterface &mBatteryInterface;
        };
    }
}
#pragma once

#include "../Service.h"
#include "../../Component/Battery/BatteryInterface.h"
#include "../../Component/Battery/BatteryObserverInterface.h"

namespace Service
{
    namespace Battery
    {
        using namespace Component::Battery;

        class ServiceBattery : public Service, public BatteryObserverInterface {
        public:
            ServiceBattery(BatteryInterface &batteryInterface,
                           Event::EventListenerInterface &eventListener);

            ~ServiceBattery() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void DispatchEvent(const SEvent &event) final override;

            virtual void UpdatedBatteryState(const BatteryState &batteryState, const uint16_t voltage) final override;

        protected:
            BatteryInterface &mBatteryInterface;
        };
    }
}

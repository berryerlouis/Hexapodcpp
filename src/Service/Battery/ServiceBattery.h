#pragma once

#include "../../Component/Battery/BatteryInterface.h"
#include "../../Core/ObserverInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Battery
    {
        using namespace Component;
        using namespace Component::Battery;

        class ServiceBattery : public Service, Core::ObserverInterface<BatteryStruct> {
        public:
            ServiceBattery(BatteryInterface              &batteryInterface,
                           Message::MessageInterface     &messageListener,
                           Event::EventListenerInterface &eventListener);

            ~ServiceBattery() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         Notified(const BatteryStruct &battery) final override;

            virtual void         DispatchEvent(const Event::Event &event) final override;

        protected:
            BatteryInterface &mBatteryInterface;
        };
    } // namespace Battery
} // namespace Service

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

        class ServiceBattery : public Service,
                               Core::ObserverInterface<BatteryStruct>,
                               Event::EventListenerInterface {
        public:
            ServiceBattery(BatteryInterface                &batteryInterface,
                           Message::MessageInterface       &messageListener,
                           Event::EventDispatcherInterface &eventDispatcher);

            ~ServiceBattery() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Notified(const BatteryStruct &battery) final override;

            virtual void OnEvent(const Event::Event &event) final override;

        protected:
            BatteryInterface &mBatteryInterface;
        };
    } // namespace Battery
} // namespace Service

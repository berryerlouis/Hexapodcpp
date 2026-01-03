#include "ServiceBattery.h"

#include "../../Cluster/Battery/ClusterBattery.h"

namespace Service
{
    namespace Battery
    {
        ServiceBattery::ServiceBattery(
                BatteryInterface                &batteryInterface,
                Message::MessageInterface       &messageListener,
                Event::EventDispatcherInterface &eventDispatcher)
            : Service(BATTERY, 100U, messageListener, eventDispatcher)
            , mBatteryInterface(batteryInterface) {
        }

        Core::Status ServiceBattery::Initialize(void) {
            const Core::Status success = this->mBatteryInterface.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->GetEventDispatcher().AddListener(this);
                this->mBatteryInterface.Attach(this);
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceBattery::Update(const uint64_t currentTime) {
            this->mBatteryInterface.Update(currentTime);
        }

        void ServiceBattery::Notified(const BatteryStruct &battery) {
            Frame response;
            Cluster::Battery::ClusterBattery::BuildFrameState(battery.state,
                                                              battery.voltage,
                                                              battery.intensity,
                                                              response);
            this->SendMessage(response);
            this->DispatchEvent<BatteryStruct>(EventType::EVENT_BATTERY_UPDATE,
                                               battery);
        }

        void ServiceBattery::OnEvent(const Event::Event &event) {
            (void) event;
        }


    } // namespace Battery
} // namespace Service

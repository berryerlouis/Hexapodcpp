#include "ServiceBattery.h"

namespace Service
{
    namespace Battery
    {
        ServiceBattery::ServiceBattery(BatteryInterface &batteryInterface,
                                       Event::EventListenerInterface &eventListener)
            : Service(100U, eventListener), mBatteryInterface(batteryInterface) {
        }

        Core::CoreStatus ServiceBattery::Initialize(void) {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
            if (this->mBatteryInterface.Initialize()) {
                this->mBatteryInterface.Attach(this);
                success = Core::CoreStatus::CORE_OK;
            }
            return (success);
        }

        void ServiceBattery::Update(const uint64_t currentTime) {
            this->mBatteryInterface.Update(currentTime);
        }

        void ServiceBattery::DispatchEvent(const SEvent &event) {
            (void) event;
        }

        void ServiceBattery::UpdatedBatteryState(const BatteryState &batteryState, const uint16_t voltage) {
            const uint8_t arg[1U] = {static_cast<uint8_t>(voltage)};
            const SEvent ev(EServices::BATTERY, static_cast<uint8_t>(batteryState), arg, 1U);
            this->AddEvent(ev);
        }
    }
}

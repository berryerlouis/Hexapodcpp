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
                success = Core::CoreStatus::CORE_OK;
            }
            this->mBatteryInterface.Attach(this);
            return (success);
        }

        void ServiceBattery::Update(const uint64_t currentTime) {
            this->mBatteryInterface.Update(currentTime);
        }

        void ServiceBattery::DispatchEvent(const SEvent &event) {
            (void) event;
        }

        void ServiceBattery::UpdatedBatteryState(const BatteryState &batteryState, const uint16_t voltage) {
            const uint8_t arg[2U] = UINT16_TO_ARRAY(voltage);
            const SEvent ev(EServices::BATTERY, static_cast<uint8_t>(batteryState), arg, 2U);
            this->AddEvent(ev);
        }
    }
}

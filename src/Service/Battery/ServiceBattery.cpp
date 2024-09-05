#include "ServiceBattery.h"

namespace Service
{
    namespace Battery
    {
        ServiceBattery::ServiceBattery(BatteryInterface &batteryInterface)
            : Service(100U), mBatteryInterface(batteryInterface) {
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

        void ServiceBattery::UpdatedBatteryState(const BatteryState &batteryState) {
            this->mComMediator->SendMessage({id: Cluster::EClusters::BATTERY, value: (uint8_t) batteryState});
        }
    }
}

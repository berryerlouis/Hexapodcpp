#include "ServiceBattery.h"

#include "../../Cluster/Battery/ClusterBattery.h"

namespace Service
{
    namespace Battery
    {
        ServiceBattery::ServiceBattery(BatteryInterface &batteryInterface,
                                       Event::MessageInterface &messageListener) :
            Service(100U, messageListener)
            , mBatteryInterface(batteryInterface) {
        }

        Core::Status ServiceBattery::Initialize(void) {
            const Core::Status success = this->mBatteryInterface.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->mBatteryInterface.Attach(this);
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceBattery::Update(const uint64_t currentTime) {
            this->mBatteryInterface.Update(currentTime);
        }

        void ServiceBattery::UpdatedBatteryState(const BatteryState &batteryState, const uint16_t voltage) {
            Frame response;
            Cluster::Battery::ClusterBattery::BuildFrameState(batteryState, voltage, response);
            this->SendMessage(response);
        }
    } // namespace Battery
} // namespace Service

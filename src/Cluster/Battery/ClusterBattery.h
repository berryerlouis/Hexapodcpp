#pragma once

#include "../ClusterBase.h"
#include "../../Component/Battery/BatteryInterface.h"

namespace Cluster
{
    namespace Battery
    {
        using namespace Component::Battery;

        class ClusterBattery : public ClusterBase {
        public:
            ClusterBattery(BatteryInterface &battery)
                : ClusterBase(BATTERY)
                  , mBattery(battery) {
            }

            ~ClusterBattery() = default;

            inline virtual Core::CoreStatus Execute(Frame &request, Frame &response) final override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

                if (request.clusterId != this->GetId()) {
                    return (success);
                }

                switch (static_cast<EBatteryCommands>(request.commandId)) {
                    case EBatteryCommands::GET_VOLTAGE:
                        success = this->BuildFrameVoltage(response);
                        break;

                    case EBatteryCommands::GET_BAT_STATUS:
                        success = this->BuildFrameState(response);
                        break;

                    default:
                        break;
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameVoltage(Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::BATTERY,
                    EBatteryCommands::GET_VOLTAGE);
                if (success) {
                    response.Set2BytesParam(this->mBattery.GetVoltage());
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameState(Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::BATTERY,
                    EBatteryCommands::GET_BAT_STATUS);
                if (success) {
                    response.Set1ByteParam(this->mBattery.GetState());
                    response.Set2BytesParam(this->mBattery.GetVoltage());
                }
                return (success);
            }

        private:
            BatteryInterface &mBattery;
        };
    }
}

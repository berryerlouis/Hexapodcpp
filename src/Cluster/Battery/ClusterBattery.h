#pragma once

#include "../ClusterBase.h"
#include "../../Component/Battery/BatteryInterface.h"

namespace Cluster
{
    namespace Battery
    {
        using namespace Component::Battery;

        class ClusterBattery : public ClusterBase, StrategyCluster {
        public:
            ClusterBattery(BatteryInterface &battery)
                : ClusterBase(BATTERY, this)
                  , StrategyCluster(NB_COMMANDS_BATTERY)
                  , mBattery(battery) {
                this->AddClusterItem({.commandId = EBatteryCommands::GET_VOLTAGE, .expectedSize = 0U});
                this->AddClusterItem({.commandId = EBatteryCommands::GET_BAT_STATUS, .expectedSize = 0U});
            }

            ~ClusterBattery() = default;

            virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                if (request.commandId == EBatteryCommands::GET_VOLTAGE) {
                    const uint16_t voltage = this->mBattery.GetVoltage();
                    success = this->BuildFrameVoltage(voltage, response);
                } else if (request.commandId == EBatteryCommands::GET_BAT_STATUS) {
                    const uint16_t voltage = this->mBattery.GetVoltage();
                    const BatteryState state = this->mBattery.GetState();
                    success = this->BuildFrameState(state, voltage, response);
                }
                return success;
            }

            inline Core::CoreStatus BuildFrameVoltage(const uint16_t voltage, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::BATTERY,
                    EBatteryCommands::GET_VOLTAGE);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set2BytesParam(voltage);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameState(const uint16_t state, const uint16_t voltage,
                                                    Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::BATTERY,
                    EBatteryCommands::GET_BAT_STATUS);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set1ByteParam(state);
                    response.Set2BytesParam(voltage);
                }
                return (success);
            }

        private:
            BatteryInterface &mBattery;
        };
    }
}

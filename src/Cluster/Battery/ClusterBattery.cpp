#include "ClusterBattery.h"


namespace Cluster
{
    namespace Battery
    {
        using namespace Component::Battery;

        ClusterBattery::ClusterBattery(BatteryInterface &battery) :
            ClusterBase(BATTERY, this)
            , ClusterCommand(NB_COMMANDS_BATTERY)
            , mBattery(battery) {
            this->AddClusterItem({.commandId = EBatteryCommands::GET_VOLTAGE, .expectedSize = 0U});
            this->AddClusterItem({.commandId = EBatteryCommands::GET_BAT_STATUS, .expectedSize = 0U});
        }

        Core::Status ClusterBattery::ExecuteFrame(const Frame &request, Frame &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.commandId == EBatteryCommands::GET_VOLTAGE) {
                const uint16_t voltage = this->mBattery.GetVoltage();
                success = BuildFrameVoltage(voltage, response);
            } else if (request.commandId == EBatteryCommands::GET_BAT_STATUS) {
                const uint16_t voltage = this->mBattery.GetVoltage();
                const BatteryState state = this->mBattery.GetState();
                success = BuildFrameState(state, voltage, response);
            }
            return success;
        }

        Core::Status ClusterBattery::BuildFrameVoltage(const uint16_t voltage, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::BATTERY,
                    EBatteryCommands::GET_VOLTAGE);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(voltage);
            }
            return (success);
        }

        Core::Status ClusterBattery::BuildFrameState(const uint16_t state, const uint16_t voltage,
                                                     Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::BATTERY,
                    EBatteryCommands::GET_BAT_STATUS);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(state);
                response.Set2BytesParam(voltage);
            }
            return (success);
        }
    };
}

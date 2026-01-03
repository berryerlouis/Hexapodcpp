#include "ClusterBattery.h"


namespace Cluster
{
    namespace Battery
    {
        using namespace Component::Battery;

        ClusterBattery::ClusterBattery(BatteryInterface &battery)
            : ClusterBase(BATTERY, *this)
            , ClusterCommand(NB_COMMANDS_BATTERY)
            , mBattery(battery) {
            this->AddClusterItem({.commandId = EBatteryCommands::GET_BAT_STATUS,
                                  .expectedSize = 0U});
            this->AddClusterItem({.commandId = EBatteryCommands::GET_VOLTAGE,
                                  .expectedSize = 0U});
            this->AddClusterItem({.commandId = EBatteryCommands::GET_CURRENT,
                                  .expectedSize = 0U});

            LOG_CLUSTER_DEBUG("Battery", "(%d) Initialized.", BATTERY);
        }

        Core::Status ClusterBattery::ExecuteFrame(const Frame &request,
                                                  Frame       &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.GetCommandId() == EBatteryCommands::GET_BAT_STATUS) {
                const uint16_t     voltage = this->mBattery.GetVoltage();
                const uint16_t     intensity = this->mBattery.GetIntensity();
                const BatteryState state = this->mBattery.GetState();
                success = BuildFrameState(state, voltage, intensity, response);
            } else if (request.GetCommandId() ==
                       EBatteryCommands::GET_VOLTAGE) {
                const uint16_t voltage = this->mBattery.GetVoltage();
                success = BuildFrameVoltage(voltage, response);
            } else if (request.GetCommandId() ==
                       EBatteryCommands::GET_CURRENT) {
                const uint16_t intensity = this->mBattery.GetIntensity();
                success = BuildFrameIntensity(intensity, response);
            }
            return success;
        }

        Core::Status ClusterBattery::BuildFrameVoltage(const uint16_t voltage,
                                                       Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::BATTERY, EBatteryCommands::GET_VOLTAGE);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(voltage);
            }
            return (success);
        }

        Core::Status
        ClusterBattery::BuildFrameIntensity(const uint16_t intensity,
                                            Frame         &response) {
            const Core::Status success = response.Build(
                    EClusters::BATTERY, EBatteryCommands::GET_CURRENT);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(intensity);
            }
            return (success);
        }

        Core::Status ClusterBattery::BuildFrameState(const uint16_t state,
                                                     const uint16_t voltage,
                                                     const uint16_t intensity,
                                                     Frame         &response) {
            const Core::Status success = response.Build(
                    EClusters::BATTERY, EBatteryCommands::GET_BAT_STATUS);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(state);
                response.Set2BytesParam(voltage);
                response.Set2BytesParam(intensity);
            }
            return (success);
        }
    }; // namespace Battery
} // namespace Cluster

#include "ClusterProximity.h"

namespace Cluster
{
    namespace Proximity
    {
        using namespace Component::Proximity;

        ClusterProximity::ClusterProximity(SensorProximityMultipleInterface &proximity) :
            ClusterBase(PROXIMITY, this)
            , ClusterCommand(NB_COMMANDS_PROXIMITY)
            , mProximity(proximity) {
            this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::US_LEFT, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::US_RIGHT, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::LASER, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::SET_THRESHOLD, .expectedSize = 3U});
        }

        Core::Status ClusterProximity::ExecuteFrame(const Frame &request, Frame &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.commandId == EProximityCommands::LASER) {
                const SensorsId sensorId = static_cast<SensorsId>(request.commandId);
                const uint16_t distance = this->mProximity.GetDistance(sensorId);
                success = this->BuildFrameDistance(sensorId, distance, response);
            } else if (request.commandId == EProximityCommands::US_LEFT) {
                const SensorsId sensorId = static_cast<SensorsId>(request.commandId);
                const uint16_t distance = this->mProximity.GetDistance(sensorId);
                success = this->BuildFrameDistance(sensorId, distance, response);
            } else if (request.commandId == EProximityCommands::US_RIGHT) {
                const SensorsId sensorId = static_cast<SensorsId>(request.commandId);
                const uint16_t distance = this->mProximity.GetDistance(sensorId);
                success = this->BuildFrameDistance(sensorId, distance, response);
            } else if (request.commandId == EProximityCommands::SET_THRESHOLD) {
                const SensorsId sensorId = static_cast<SensorsId>(request.Get1ByteParam(0U));
                const uint16_t threshold = request.Get2BytesParam(1U);
                this->mProximity.SetThreshold(sensorId, threshold);
                success = this->BuildFrameThreshold(sensorId, threshold, response);
            }
            return success;
        }

        Core::Status ClusterProximity::BuildFrameDistance(const SensorsId sensorId, const uint16_t distance,
                                                          Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::PROXIMITY,
                    sensorId);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(distance);
            }
            return (success);
        }

        Core::Status ClusterProximity::BuildFrameThreshold(const SensorsId sensorId, const uint16_t threshold,
                                                           Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::PROXIMITY,
                    EProximityCommands::SET_THRESHOLD);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(sensorId);
                response.Set2BytesParam(threshold);
            }
            return (success);
        }
    };
}

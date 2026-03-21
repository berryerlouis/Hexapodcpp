#include "ClusterProximity.h"

namespace Cluster
{
    namespace Proximity
    {
        using namespace Component::Proximity;

        ClusterProximity::ClusterProximity(SensorProximityMultipleInterface &proximity)
            : ClusterBase(PROXIMITY, *this)
            , ClusterCommand(NB_COMMANDS_PROXIMITY)
            , mProximity(proximity) {
            this->AddClusterItem(ClusterItem(EProximityCommands::US_LEFT, 0U));
            this->AddClusterItem(ClusterItem(EProximityCommands::US_RIGHT, 0U));
            this->AddClusterItem(ClusterItem(EProximityCommands::LASER, 0U));
            this->AddClusterItem(ClusterItem(EProximityCommands::SET_THRESHOLD, 3U));
            LOG_CLUSTER_DEBUG("Proximity", "(%d) Initialized.", PROXIMITY);
        }

        Core::Status ClusterProximity::ExecuteFrame(const Frame &request, Frame &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.GetCommandId() == EProximityCommands::LASER) {
                const SensorsId sensorId = static_cast<SensorsId>(request.GetCommandId());
                const uint16_t  distance = this->mProximity.GetDistance(sensorId);
                success = BuildFrameDistanceUS(sensorId, distance, response);
            } else if (request.GetCommandId() == EProximityCommands::US_LEFT) {
                const SensorsId sensorId = static_cast<SensorsId>(request.GetCommandId());
                const uint16_t  distance = this->mProximity.GetDistance(sensorId);
                success = BuildFrameDistanceUS(sensorId, distance, response);
            } else if (request.GetCommandId() == EProximityCommands::US_RIGHT) {
                const SensorsId sensorId = static_cast<SensorsId>(request.GetCommandId());
                const uint16_t  distance = this->mProximity.GetDistance(sensorId);
                success = BuildFrameDistanceUS(sensorId, distance, response);
            } else if (request.GetCommandId() == EProximityCommands::SET_THRESHOLD) {
                const SensorsId sensorId = static_cast<SensorsId>(request.Get1ByteParam(0U));
                const uint16_t  threshold = request.Get2BytesParam(1U);
                this->mProximity.SetThreshold(sensorId, threshold);
                success = BuildFrameThreshold(sensorId, threshold, response);
            }
            return success;
        }

        Core::Status ClusterProximity::BuildFrameDistanceUS(const SensorsId sensorId,
                                                            const uint16_t  distance,
                                                            Frame          &response) {
            const Core::Status success = response.Build(EClusters::PROXIMITY, sensorId);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(distance);
            }
            return (success);
        }

        Core::Status
        ClusterProximity::BuildFrameDistanceVLX(const SensorsId sensorId,
                                                const uint16_t (&distanceMatrix)[8U][8U],
                                                Frame &response) {
            const Core::Status success = response.Build(EClusters::PROXIMITY, sensorId);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(64U * 2U,
                                        reinterpret_cast<const uint8_t *>(distanceMatrix));
            }
            return (success);
        }

        Core::Status ClusterProximity::BuildFrameThreshold(const SensorsId sensorId,
                                                           const uint16_t  threshold,
                                                           Frame          &response) {
            const Core::Status success =
                    response.Build(EClusters::PROXIMITY, EProximityCommands::SET_THRESHOLD);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(sensorId);
                response.Set2BytesParam(threshold);
            }
            return (success);
        }
    }; // namespace Proximity
} // namespace Cluster

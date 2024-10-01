#pragma once

#include "../ClusterBase.h"
#include "../../Component/Proximity/SensorProximityInterface.h"

namespace Cluster
{
    namespace Proximity
    {
        using namespace Component::Proximity;

        class ClusterProximity : public ClusterBase, StrategyCluster {
        public:
            ClusterProximity(SensorProximityMultipleInterface &proximity)
                : ClusterBase(PROXIMITY, this)
                  , StrategyCluster(NB_COMMANDS_PROXIMITY)
                  , mProximity(proximity) {
                this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::US_LEFT, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::US_RIGHT, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::LASER, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::SET_THRESHOLD, .expectedSize = 3U});
            }

            ~ClusterProximity() = default;

            virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
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

            inline Core::CoreStatus BuildFrameDistance(const SensorsId sensorId, const uint16_t distance,
                                                       Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::PROXIMITY,
                    sensorId);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set2BytesParam(distance);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameThreshold(const SensorsId sensorId, const uint16_t threshold,
                                                        Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::PROXIMITY,
                    EProximityCommands::SET_THRESHOLD);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set1ByteParam(sensorId);
                    response.Set2BytesParam(threshold);
                }
                return (success);
            }

        private:
            SensorProximityMultipleInterface &mProximity;
        };
    }
}

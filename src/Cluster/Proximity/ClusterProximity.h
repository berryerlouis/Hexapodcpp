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
                  , mProximity(proximity) {
                this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::US_LEFT, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::US_RIGHT, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::LASER, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EProximityCommands::SET_THRESHOLD, .expectedSize = 0U});
            }

            ~ClusterProximity() = default;

            virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                if (request.commandId == EProximityCommands::LASER) {
                    success = this->BuildFrameDistance(static_cast<SensorsId>(request.commandId), response);
                } else if (request.commandId == EProximityCommands::US_LEFT) {
                    success = this->BuildFrameDistance(static_cast<SensorsId>(request.commandId), response);
                } else if (request.commandId == EProximityCommands::US_RIGHT) {
                    success = this->BuildFrameDistance(static_cast<SensorsId>(request.commandId), response);
                } else if (request.commandId == EProximityCommands::SET_THRESHOLD) {
                    const SensorsId sensorId = static_cast<SensorsId>(request.Get1ByteParam(0U));
                    const uint16_t threshold = request.Get2BytesParam(1U);
                    this->mProximity.SetThreshold(sensorId, threshold);
                    success = this->BuildFrameThreshold(sensorId, response);
                }
                return success;
            }

            inline Core::CoreStatus BuildFrameDistance(const SensorsId sensorId, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::PROXIMITY,
                    sensorId);
                if (success) {
                    response.Set2BytesParam(this->mProximity.GetDistance(sensorId));
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameThreshold(const SensorsId sensorId, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::PROXIMITY,
                    EProximityCommands::SET_THRESHOLD);
                if (success) {
                    response.Set2BytesParam(this->mProximity.GetThreshold(sensorId));
                }
                return (success);
            }

        private:
            SensorProximityMultipleInterface &mProximity;
        };
    }
}

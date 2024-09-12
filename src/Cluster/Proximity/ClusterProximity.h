#pragma once

#include "../ClusterBase.h"
#include "../../Component/Proximity/SensorProximityInterface.h"

namespace Cluster
{
    namespace Proximity
    {
        using namespace Component::Proximity;

        class ClusterProximity : public ClusterBase {
        public:
            ClusterProximity(SensorProximityMultipleInterface &proximity)
                : ClusterBase(PROXIMITY)
                  , mProximity(proximity) {
            }

            ~ClusterProximity() = default;

            inline virtual Core::CoreStatus Execute(Frame &request, Frame &response) final override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

                if (request.clusterId != this->GetId()) {
                    return (success);
                }

                switch ((EProximityCommands) request.commandId) {
                    case EProximityCommands::US_LEFT: {
                        success = this->BuildFrameDistance((EProximityCommands) request.commandId, response);
                    }
                    break;

                    case EProximityCommands::US_RIGHT: {
                        success = this->BuildFrameDistance((EProximityCommands) request.commandId, response);
                    }
                    break;

                    case EProximityCommands::LASER: {
                        success = this->BuildFrameDistance((EProximityCommands) request.commandId, response);
                    }
                    break;

                    case EProximityCommands::SET_THRESHOLD: {
                        uint8_t sensorId = request.Get1ByteParam(0U);
                        uint16_t threshold = request.Get2BytesParam(1U);
                        success = this->mProximity.SetThreshold((SensorsId) sensorId, threshold);
                        if (success == true) {
                            success = this->BuildFrameThreshold((EProximityCommands) request.commandId, response);
                        }
                    }
                    break;

                    default:
                        break;
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameDistance(EProximityCommands sensorId, Frame &response) const {
                Core::CoreStatus success = response.Build(
                    EClusters::PROXIMITY,
                    sensorId);
                if (success) {
                    response.Set2BytesParam(this->mProximity.GetDistance((SensorsId) sensorId));
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameThreshold(EProximityCommands sensorId, Frame &response) const {
                Core::CoreStatus success = response.Build(
                    EClusters::PROXIMITY,
                    EProximityCommands::SET_THRESHOLD);
                if (success) {
                    response.Set2BytesParam(this->mProximity.GetThreshold((SensorsId) sensorId));
                }
                return (success);
            }

        private:
            SensorProximityMultipleInterface &mProximity;
        };
    }
}

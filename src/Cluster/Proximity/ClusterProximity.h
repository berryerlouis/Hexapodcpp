#pragma once

#include "../ClusterBase.h"
#include "../../Component/Proximity/SensorProximityInterface.h"

namespace Cluster
{
    namespace Proximity
    {
        using namespace Component::Proximity;

        class ClusterProximity : public ClusterBase, ClusterCommand {
        public:
            ClusterProximity(SensorProximityMultipleInterface &proximity);

            ~ClusterProximity() = default;

            virtual Core::Status ExecuteFrame(const Frame &request, Frame &response) override;

            static Core::Status BuildFrameDistance(const SensorsId sensorId, const uint16_t distance,
                                                   Frame &response);

            static Core::Status BuildFrameThreshold(const SensorsId sensorId, const uint16_t threshold,
                                                    Frame &response);

        private:
            SensorProximityMultipleInterface &mProximity;
        };
    }
}

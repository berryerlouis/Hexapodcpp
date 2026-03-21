#pragma once

#include "../../Component/Proximity/SensorProximityInterface.h"
#include "../ClusterBase.h"

namespace Cluster
{
    namespace Proximity
    {
        using namespace Component::Proximity;

        class ClusterProximity : public ClusterBase, ClusterCommand {
        public:
            explicit ClusterProximity(SensorProximityMultipleInterface &proximity);

            ~ClusterProximity() = default;

            virtual Core::Status ExecuteFrame(const Frame &request, Frame &response) override;

            static Core::Status  BuildFrameDistanceUS(const SensorsId sensorId,
                                                      const uint16_t  distance,
                                                      Frame          &response);

            static Core::Status  BuildFrameDistanceVLX(const SensorsId sensorId,
                                                       const uint16_t (&distanceMatrix)[8U][8U],
                                                       Frame &response);

            static Core::Status  BuildFrameThreshold(const SensorsId sensorId,
                                                     const uint16_t  threshold,
                                                     Frame          &response);

        private:
            SensorProximityMultipleInterface &mProximity;
        };
    } // namespace Proximity
} // namespace Cluster

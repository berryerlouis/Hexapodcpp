#pragma once

#include "SensorsId.h"

namespace Component
{
    namespace Proximity
    {
        class SensorProximityObserverInterface {
        public:
            SensorProximityObserverInterface() = default;

            ~SensorProximityObserverInterface() = default;

            virtual void Detect(const SensorsId &sensorId, const uint16_t &distance) = 0;
        };
    }
}

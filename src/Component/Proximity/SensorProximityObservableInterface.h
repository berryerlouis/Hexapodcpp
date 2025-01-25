#pragma once

#include "SensorProximityObserverInterface.h"

namespace Component
{
    namespace Proximity
    {
        class SensorProximityObservableInterface {
        public:
            SensorProximityObservableInterface() = default;

            ~SensorProximityObservableInterface() = default;

            virtual Core::Status Attach(SensorProximityObserverInterface *observer) = 0;

            virtual void Notify(const SensorsId &sensorId, const uint16_t distance) = 0;
        };
    }
}

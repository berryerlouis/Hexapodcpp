#pragma once

#include "../ComponentInterface.h"
#include "SensorProximityObservable.h"

namespace Component
{
    namespace Proximity
    {
        class SensorProximityInterface : public ComponentInterface, public SensorProximityObservable {
        public:
            SensorProximityInterface(void) = default;

            ~SensorProximityInterface() = default;

            virtual Core::CoreStatus Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual uint16_t GetDistance(void) = 0;

            virtual uint16_t GetThreshold(void) = 0;

            virtual Core::CoreStatus SetThreshold(const uint16_t threshold) = 0;
        };

        class SensorProximityMultipleInterface : public ComponentInterface {
        public:
            SensorProximityMultipleInterface(void) = default;

            ~SensorProximityMultipleInterface() = default;

            virtual Core::CoreStatus Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual uint16_t GetDistance(const SensorsId &sensorId) = 0;

            virtual uint16_t GetThreshold(const SensorsId &sensorId) = 0;

            virtual Core::CoreStatus SetThreshold(const SensorsId &sensorId, const uint16_t threshold) = 0;

            virtual Core::CoreStatus Attach(SensorProximityObserverInterface *observer) = 0;
        };
    }
}

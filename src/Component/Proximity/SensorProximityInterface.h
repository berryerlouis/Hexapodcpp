#pragma once

#include "../ComponentInterface.h"
#include "SensorsId.h"

namespace Component
{
    namespace Proximity
    {
        class SensorProximityInterface : public ComponentInterface<5U, SensorsStruct> {
        public:
            SensorProximityInterface(void) = default;

            ~SensorProximityInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual uint16_t GetDistance(void) = 0;

            virtual uint16_t GetThreshold(void) = 0;

            virtual Core::Status SetThreshold(const uint16_t threshold) = 0;
        };

        class SensorProximityMultipleInterface : public ComponentInterface<5U, SensorsStruct> {
        public:
            SensorProximityMultipleInterface(void) = default;

            ~SensorProximityMultipleInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual uint16_t GetDistance(const SensorsId &sensorId) = 0;

            virtual uint16_t GetThreshold(const SensorsId &sensorId) = 0;

            virtual Core::Status SetThreshold(const SensorsId &sensorId, const uint16_t threshold) = 0;
        };
    }
}
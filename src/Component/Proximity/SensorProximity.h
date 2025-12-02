#pragma once

#include "../../Cluster/Constants.h"
#include "SensorProximityInterface.h"

namespace Component
{
    namespace Proximity
    {
        class SensorProximity : public SensorProximityMultipleInterface, Core::ObserverInterface<SensorsStruct> {
        public:
            static constexpr uint8_t NB_SENSORS = 3U;

            SensorProximity(SensorProximityInterface &srf05Left, SensorProximityInterface &srf05Right,
                            SensorProximityInterface &Vl53l0x);

            ~SensorProximity() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual uint16_t GetDistance(const SensorsId &sensorId) final override;

            virtual Core::Status SetThreshold(const SensorsId &sensorId, const uint16_t threshold) final override;

            virtual uint16_t GetThreshold(const SensorsId &sensorId) final override;

            virtual void Notified(const SensorsStruct &sensor) final override;

        private:
            SensorProximityInterface *mSensors[NB_SENSORS];
        };
    } // namespace Proximity
} // namespace Component
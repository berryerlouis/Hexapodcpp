#include "SensorProximity.h"

namespace Component
{
    namespace Proximity
    {
        SensorProximity::SensorProximity(SensorProximityInterface &srf05Left, SensorProximityInterface &srf05Right,
                                         SensorProximityInterface &Vl53l0x) :
            mSensors{&srf05Left, &srf05Right, &Vl53l0x} {
        }

        Core::Status SensorProximity::Initialize(void) {
            uint8_t success = 0U;

            for (SensorProximityInterface *sensor: this->mSensors) {
                if (sensor->Initialize() == Core::Status::CORE_OK) { success++; }
            }

            return ((success == NB_SENSORS - 1U) ? Core::Status::CORE_OK : Core::Status::CORE_ERROR);
        }

        void SensorProximity::Update(const uint64_t currentTime) {
            for (size_t sensorId = 0; sensorId < NB_SENSORS; sensorId++) {
                this->mSensors[sensorId]->Update(currentTime);
            }
        }

        uint16_t SensorProximity::GetDistance(const SensorsId &sensorId) {
            return (this->mSensors[sensorId]->GetDistance());
        }

        Core::Status SensorProximity::SetThreshold(const SensorsId &sensorId, const uint16_t threshold) {
            return (this->mSensors[sensorId]->SetThreshold(threshold));
        }

        uint16_t SensorProximity::GetThreshold(const SensorsId &sensorId) {
            return (this->mSensors[sensorId]->GetThreshold());
        }

        Core::Status SensorProximity::Attach(SensorProximityObserverInterface *observer) {
            uint8_t success = 0U;

            for (SensorProximityInterface *sensor: this->mSensors) {
                if (sensor->Attach(observer) == Core::Status::CORE_OK) { success++; }
            }

            return ((success == NB_SENSORS) ? Core::Status::CORE_OK : Core::Status::CORE_ERROR);
        }
    }
}

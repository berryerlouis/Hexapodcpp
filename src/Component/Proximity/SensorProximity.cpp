#include "SensorProximity.h"

namespace Component
{
    namespace Proximity
    {
        SensorProximity::SensorProximity(SensorProximityInterface &srf05Left,
                                         SensorProximityInterface &srf05Right,
                                         SensorProximityInterface &vl53lxx)
            : mSensors{&srf05Left, &srf05Right, &vl53lxx}
            , mReady{false, false, false}
            , mNextUltrasound(SRF_LEFT) {
        }

        Core::Status SensorProximity::Initialize() {
            uint8_t success = 0U;

            for (uint8_t id = 0U; id < NB_SENSORS; id++) {
                SensorProximityInterface *sensor = this->mSensors[id];
                if (sensor->Initialize() == Core::Status::CORE_OK) {
                    sensor->Attach(this);
                    this->mReady[id] = true;
                    success++;
                }
            }

            LOG_COMPONENT_DEBUG("SensorProximity", "Initialized.");
            return ((success >= NB_SENSORS - 1U) ? Core::Status::CORE_OK
                                                 : Core::Status::CORE_ERROR);
        }

        void SensorProximity::Notified(const SensorsStruct &sensor) {
            this->Notify(sensor);
        }

        void SensorProximity::Update(const uint64_t currentTime) {
            // The ToF has no acoustic crosstalk, so it is polled every tick; the two ultrasound
            // sensors take turns.
            if (this->mReady[VLX]) {
                this->mSensors[VLX]->Update(currentTime);
            }
            if (this->mReady[this->mNextUltrasound]) {
                this->mSensors[this->mNextUltrasound]->Update(currentTime);
            }
            this->mNextUltrasound = (this->mNextUltrasound == SRF_LEFT) ? SRF_RIGHT : SRF_LEFT;
        }

        uint16_t SensorProximity::GetDistance(const SensorsId &sensorId) {
            return (this->mSensors[sensorId]->GetDistance());
        }

        Core::Status SensorProximity::SetThreshold(const SensorsId &sensorId,
                                                   const uint16_t   threshold) {
            return (this->mSensors[sensorId]->SetThreshold(threshold));
        }

        uint16_t SensorProximity::GetThreshold(const SensorsId &sensorId) {
            return (this->mSensors[sensorId]->GetThreshold());
        }
    } // namespace Proximity
} // namespace Component

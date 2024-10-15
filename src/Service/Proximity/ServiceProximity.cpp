#include "ServiceProximity.h"

namespace Service
{
    namespace Proximity
    {
        ServiceProximity::ServiceProximity(SensorProximityMultipleInterface &proximity,
                                           Event::EventListenerInterface &eventListener)
            : Service(25U, eventListener)
              , mProximity(proximity)
              , mTimeoutDetection{0xFFU, 0xFFU, 0xFFU} {
        }

        Core::CoreStatus ServiceProximity::Initialize(void) {
            /*Core::CoreStatus success = */this->mProximity.Initialize();
            this->mProximity.Attach(this);
            return (Core::CoreStatus::CORE_OK);
        }

        void ServiceProximity::Update(const uint64_t currentTime) {
            this->mProximity.Update(currentTime);

            for (int i = 0; i < NB_SENSORS; ++i) {
                if (this->mTimeoutDetection[i] < MAX_TIMEOUT_DETECTION) {
                    this->mTimeoutDetection[i]++;
                } else if (this->mTimeoutDetection[i] == MAX_TIMEOUT_DETECTION) {
                    this->mTimeoutDetection[i] = 0xFFU;
                    const uint16_t distance = this->mProximity.GetDistance(static_cast<SensorsId>(i));
                    const uint8_t arg[2U] = UINT16_TO_ARRAY(distance);
                    const SEvent ev(EServices::PROXIMITY, static_cast<uint8_t>(i), arg, 2U);
                    this->AddEvent(ev);
                }
            }
        }

        void ServiceProximity::Detect(const SensorsId &sensorId, const uint16_t distance) {
            const uint8_t arg[2U] = UINT16_TO_ARRAY(distance);
            const SEvent ev(EServices::PROXIMITY, static_cast<uint8_t>(sensorId), arg, 2U);
            this->AddEvent(ev);
            this->mTimeoutDetection[sensorId] = 0U;
        }


        void ServiceProximity::DispatchEvent(const SEvent &event) {
            (void) event;
        }
    }
}

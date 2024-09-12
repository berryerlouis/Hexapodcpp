#include "ServiceProximity.h"

namespace Service
{
    namespace Proximity
    {
        ServiceProximity::ServiceProximity(SensorProximityMultipleInterface &proximity,
                                           Event::EventListener &eventListener)
            : Service(25, eventListener)
              , mProximity(proximity)
              , mTimeoutDetection{0xFFU, 0xFFU, 0xFFU} {
        }

        Core::CoreStatus ServiceProximity::Initialize(void) {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
            if (this->mProximity.Initialize()) {
                this->mProximity.Attach(this);
                success = Core::CoreStatus::CORE_OK;
            }
            return (success);
        }

        void ServiceProximity::Update(const uint64_t currentTime) {
            this->mProximity.Update(currentTime);

            for (int i = 0; i < NB_SENSORS; ++i) {
                if (this->mTimeoutDetection[i] < MAX_TIMEOUT_DETECTION) {
                    this->mTimeoutDetection[i]++;
                } else if (this->mTimeoutDetection[i] == MAX_TIMEOUT_DETECTION) {
                    this->mTimeoutDetection[i] = 0xFFU;
                    const uint16_t distance = this->mProximity.GetDistance(static_cast<SensorsId>(i));
                    const uint8_t arg[2U] = {
                        static_cast<uint8_t>(distance >> 8U),
                        static_cast<uint8_t>(distance & 0xFFU)
                    };
                    const SEvent ev(EServices::PROXIMITY, static_cast<uint8_t>(i), arg, 2U);
                    this->AddEvent(ev);
                }
            }
        }

        void ServiceProximity::Detect(const SensorsId &sensorId, const uint16_t &distance) {
            const uint8_t arg[2U] = {
                static_cast<uint8_t>(distance >> 8U),
                static_cast<uint8_t>(distance & 0xFFU)
            };
            const SEvent ev(EServices::PROXIMITY, static_cast<uint8_t>(sensorId), arg, 2U);
            this->AddEvent(ev);
            this->mTimeoutDetection[sensorId] = 0U;
        }


        void ServiceProximity::DispatchEvent(SEvent &event) {
            (void) event;
        }
    }
}

#pragma once

#include "../../Component/Proximity/SensorProximityInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Proximity
    {
        using namespace Component::Proximity;

        constexpr uint8_t MAX_TIMEOUT_DETECTION = 10U;

        class ServiceProximity : public Service, public SensorProximityObserverInterface {
        public:
            ServiceProximity(SensorProximityMultipleInterface &proximity,
                             Event::EventListenerInterface &eventListener);

            ~ServiceProximity() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void DispatchEvent(const SEvent &event) final override;

            virtual void Detect(const SensorsId &sensorId, const uint16_t distance) final override;

        protected:
            SensorProximityMultipleInterface &mProximity;
            uint8_t mTimeoutDetection[NB_SENSORS];
        };
    }
}

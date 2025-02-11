#pragma once

#include "../../Component/Proximity/SensorProximityInterface.h"
#include "../Service.h"
#include "../../Component/ObserverInterface.h"

namespace Service
{
    namespace Proximity
    {
        using namespace Component;
        using namespace Component::Proximity;

        constexpr uint8_t MAX_TIMEOUT_DETECTION = 10U;

        class ServiceProximity : public Service, ObserverInterface<SensorsStruct> {
        public:
            ServiceProximity(SensorProximityMultipleInterface &proximity,
                             Event::MessageInterface &messageListener);

            ~ServiceProximity() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Notified(const SensorsStruct &sensor) final override;

        protected:
            SensorProximityMultipleInterface &mProximity;
            uint8_t mTimeoutDetection[NB_SENSORS];
        };
    }
}

#pragma once

#include "../../Component/Proximity/SensorProximityInterface.h"
#include "../../Core/ObserverInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Proximity
    {
        using namespace Component;
        using namespace Component::Proximity;

        constexpr uint8_t MAX_TIMEOUT_DETECTION = 10U;

        class ServiceProximity : public Service, Core::ObserverInterface<SensorsStruct> {
        public:
            ServiceProximity(SensorProximityMultipleInterface &proximity,
                             Message::MessageInterface        &messageListener,
                             Event::EventListenerInterface    &eventListener);

            ~ServiceProximity() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         Notified(const SensorsStruct &sensor) final override;

            virtual void         DispatchEvent(const Event::Event &event) final override;

        protected:
            SensorProximityMultipleInterface &mProximity;
        };
    } // namespace Proximity
} // namespace Service

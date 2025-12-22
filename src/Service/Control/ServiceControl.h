#pragma once

#include "../../Component/Servos/ServosInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Control
    {
        using namespace Component::Servos;

        class ServiceControl : public Service, Event::EventListenerInterface {
        public:
            ServiceControl(ServosInterface                 &servos,
                           Message::MessageInterface       &messageListener,
                           Event::EventDispatcherInterface &eventDispatcher);

            ~ServiceControl() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         OnEvent(const Event::Event &event) final override;

        protected:
            uint8_t          mStepPca9685;
            ServosInterface &mServosInterface;
        };
    } // namespace Control
} // namespace Service

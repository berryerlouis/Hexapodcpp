#pragma once

#include "../../Bot/Body/BodyInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Body
    {
        using namespace Bot::Body;

        class ServiceBody : public Service, Event::EventListenerInterface {
        public:
            ServiceBody(BodyInterface                   &body,
                        Message::MessageInterface       &messageListener,
                        Event::EventDispatcherInterface &eventDispatcher);

            ~ServiceBody() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void OnEvent(const Event::Event &event) final override;

        protected:
            BodyInterface &mBody;
        };
    } // namespace Body
} // namespace Service

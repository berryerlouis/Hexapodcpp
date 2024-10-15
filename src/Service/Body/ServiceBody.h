#pragma once

#include "../Service.h"
#include "../../Bot/Body/BodyInterface.h"

namespace Service
{
    namespace Body
    {
        using namespace Bot::Body;

        class ServiceBody : public Service {
        public:
            ServiceBody(BodyInterface &body, Event::EventListenerInterface &eventListener);

            ~ServiceBody() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void DispatchEvent(const SEvent &event) final override;

        protected:
            BodyInterface &mBody;
        };
    }
}

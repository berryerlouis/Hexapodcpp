#include "ServiceBody.h"

namespace Service
{
    namespace Body
    {
        ServiceBody::ServiceBody(BodyInterface &body,
                                 Event::EventListenerInterface &eventListener)
            : Service(100U, eventListener)
              , mBody(body) {
        }

        Core::CoreStatus ServiceBody::Initialize(void) {
            return (this->mBody.Initialize());
        }

        void ServiceBody::Update(const uint64_t currentTime) {
            this->mBody.Update(currentTime);
        }

        void ServiceBody::DispatchEvent(const SEvent &event) {
            (void) event;
        }
    }
}

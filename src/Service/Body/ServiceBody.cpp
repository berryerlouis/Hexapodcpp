#include "ServiceBody.h"

namespace Service
{
    namespace Body
    {
        ServiceBody::ServiceBody(BodyInterface                 &body,
                                 Message::MessageInterface     &messageListener,
                                 Event::EventListenerInterface &eventListener) :
            Service(BODY,
                    20U,
                    messageListener,
                    eventListener),
            mBody(body) {
        }

        Core::Status ServiceBody::Initialize(void) {
            const Core::Status success = this->mBody.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceBody::Update(const uint64_t currentTime) {
            this->mBody.Update(currentTime);
        }

        void ServiceBody::DispatchEvent(const Event::Event &event) const {
            (void) event;
        }
    } // namespace Body
} // namespace Service

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

        void ServiceBody::DispatchEvent(const Event::Event &event) {
            if (event.serviceId == EServices::BUTTON) {
                if (event.eventType == Event::Event::EVENT_BUTTON_PRESSED) {
                    if (this->mBody.GetWalkStatus() == Move::Walk::EWalkStatus::WALKING) {
                        this->mBody.UpdateWalkStatus(Move::Walk::EWalkStatus::STOPPED, 1000U);
                    }
                }
            }
        }
    } // namespace Body
} // namespace Service

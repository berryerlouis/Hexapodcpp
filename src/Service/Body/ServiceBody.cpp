#include "ServiceBody.h"

#include "../../Component/Button/Button.h"

namespace Service
{
    namespace Body
    {
        ServiceBody::ServiceBody(
                BodyInterface                   &body,
                Message::MessageInterface       &messageListener,
                Event::EventDispatcherInterface &eventDispatcher)
            : Service(BODY, 20U, messageListener, eventDispatcher)
            , mBody(body) {
        }

        Core::Status ServiceBody::Initialize() {
            const Core::Status success = this->mBody.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->GetEventDispatcher().AddListener(this);
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceBody::Update(const uint64_t currentTime) {
            this->mBody.Update(currentTime);
        }

        void ServiceBody::OnEvent(const Event::Event &event) {
            if (event.serviceId == EServices::BUTTON) {
                if (event.eventType == EventType::EVENT_BUTTON_UPDATE) {
                    if (event.eventArg.type() ==
                        typeid(Component::Button::ButtonStruct)) {
                        const Component::Button::ButtonStruct buttonStruct =
                                std::any_cast<Component::Button::ButtonStruct>(
                                        event.eventArg);
                        if (buttonStruct.state ==
                            Component::Button::ButtonState::PUSH) {
                            if (this->mBody.GetWalkStatus() ==
                                Move::Walk::EWalkStatus::WALKING) {
                                this->mBody.UpdateWalkStatus(
                                        Move::Walk::EWalkStatus::STOPPED,
                                        1000U);
                            }
                        }
                    }
                }
            }
        }
    } // namespace Body
} // namespace Service

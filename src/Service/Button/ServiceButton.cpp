#include "ServiceButton.h"

#include "../../Cluster/Button/ClusterButton.h"

namespace Service
{
    namespace Button
    {
        ServiceButton::ServiceButton(
                ButtonInterface                 &ButtonInterface,
                Message::MessageInterface       &messageListener,
                Event::EventDispatcherInterface &eventDispatcher)
            : Service(BUTTON, 10U, messageListener, eventDispatcher)
            , mButtonInterface(ButtonInterface) {
        }

        Core::Status ServiceButton::Initialize() {
            const Core::Status success = this->mButtonInterface.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->GetEventDispatcher().AddListener(this);
                this->mButtonInterface.Attach(this);
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceButton::Update(const uint64_t currentTime) {
            this->mButtonInterface.Update(currentTime);
        }

        void ServiceButton::Notified(const ButtonStruct &button) {
            Frame response;
            Cluster::Button::ClusterButton::BuildFrameGetButtonState(
                    button.state, response);
            this->SendMessage(response);
            this->DispatchEvent<ButtonStruct>(EventType::EVENT_BUTTON_UPDATE,
                                              button);
        }

        void ServiceButton::OnEvent(const Event::Event &event) {
            (void) event;
        }
    } // namespace Button
} // namespace Service

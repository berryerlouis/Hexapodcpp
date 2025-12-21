#include "ServiceButton.h"

#include "../../Cluster/Button/ClusterButton.h"

namespace Service
{
    namespace Button
    {
        ServiceButton::ServiceButton(ButtonInterface               &ButtonInterface,
                                     Message::MessageInterface     &messageListener,
                                     Event::EventListenerInterface &eventListener) :
            Service(BUTTON,
                    10U,
                    messageListener,
                    eventListener),
            mButtonInterface(ButtonInterface) {
        }

        Core::Status ServiceButton::Initialize(void) {
            const Core::Status success = this->mButtonInterface.Initialize();
            if (Core::Status::CORE_OK == success) {
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
            Cluster::Button::ClusterButton::BuildFrameGetButtonState(button.state, response);
            this->SendMessage(response);
            this->SetEvent((button.state == PUSH) ? Event::Event::EVENT_BUTTON_PRESSED
                                                  : Event::Event::EVENT_BUTTON_RELEASED);
        }

        void ServiceButton::DispatchEvent(const Event::Event &event) const {
            (void) event;
        }
    } // namespace Button
} // namespace Service

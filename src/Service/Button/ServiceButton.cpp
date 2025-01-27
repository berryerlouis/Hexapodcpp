#include "ServiceButton.h"

#include "../../Cluster/Button/ClusterButton.h"

namespace Service
{
    namespace Button
    {
        ServiceButton::ServiceButton(ButtonInterface &ButtonInterface,
                                     Event::EventListenerInterface &eventListener) :
            Service(100U, eventListener), mButtonInterface(ButtonInterface) {
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

        void ServiceButton::UpdatedButtonState(const ButtonState &ButtonState, const uint16_t period) {
            Frame response;
            Cluster::Button::ClusterButton::BuildFrameGetButtonState(ButtonState, response);
            this->SendMessage(response);
        }
    } // namespace Button
} // namespace Service

#include "ServiceButton.h"

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

        void ServiceButton::DispatchEvent(const SEvent &event) {
            (void) event;
        }

        void ServiceButton::UpdatedButtonState(const ButtonState &ButtonState, const uint16_t period) {
            const uint8_t arg[2U] = UINT16_TO_ARRAY(period);
            const SEvent ev(EServices::BUTTON, static_cast<uint8_t>(ButtonState), arg, 2U);
            this->AddEvent(ev);
        }
    } // namespace Button
} // namespace Service

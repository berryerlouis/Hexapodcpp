#pragma once

#include "../Service.h"
#include "../../Component/Button/ButtonInterface.h"
#include "../../Component/Button/ButtonObserverInterface.h"

namespace Service
{
    namespace Button
    {
        using namespace Component::Button;

        class ServiceButton : public Service, public ButtonObserverInterface {
        public:
            ServiceButton(ButtonInterface &ButtonInterface,
                          Event::EventListenerInterface &eventListener);

            ~ServiceButton() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void UpdatedButtonState(const ButtonState &ButtonState, const uint16_t period) final override;

        protected:
            ButtonInterface &mButtonInterface;
        };
    }
}

#pragma once

#include "../../Component/Button/ButtonInterface.h"
#include "../../Core/ObserverInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Button
    {
        using namespace Component;
        using namespace Component::Button;

        class ServiceButton : public Service, Core::ObserverInterface<ButtonStruct> {
        public:
            ServiceButton(ButtonInterface               &ButtonInterface,
                          Message::MessageInterface     &messageListener,
                          Event::EventListenerInterface &eventListener);

            ~ServiceButton() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         Notified(const ButtonStruct &button) final override;

            virtual void         DispatchEvent(const Event::Event &event) const final override;

        protected:
            ButtonInterface &mButtonInterface;
        };
    } // namespace Button
} // namespace Service

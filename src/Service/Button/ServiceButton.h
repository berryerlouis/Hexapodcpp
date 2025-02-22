#pragma once

#include "../Service.h"
#include "../../Component/Button/ButtonInterface.h"
#include "../../Core/ObserverInterface.h"

namespace Service
{
    namespace Button
    {
        using namespace Component;
        using namespace Component::Button;

        class ServiceButton : public Service, Core::ObserverInterface<ButtonStruct> {
        public:
            ServiceButton(ButtonInterface &ButtonInterface,
                          Event::MessageInterface &messageListener);

            ~ServiceButton() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Notified(const ButtonStruct &button) final override;

        protected:
            ButtonInterface &mButtonInterface;
        };
    }
}

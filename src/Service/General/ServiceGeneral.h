#pragma once

#include "../../Component/LedPwm/LedPwmInterface.h"
#include "../../Component/Software/SoftwareInterface.h"
#include "../Service.h"


namespace Service
{
    namespace General
    {
        using namespace Component::Software;
        using namespace Component::LedPwm;

        class ServiceGeneral : public Service, Event::EventListenerInterface {
        public:
            ServiceGeneral(LedPwmInterface                 &ledPwm,
                           SoftwareInterface               &software,
                           Message::MessageInterface       &messageListener,
                           Event::EventDispatcherInterface &eventDispatcher);

            ~ServiceGeneral() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         OnEvent(const Event::Event &event) final override;

        protected:
            LedPwmInterface   &mLedPwm;
            SoftwareInterface &mSoftware;
        };
    } // namespace General
} // namespace Service

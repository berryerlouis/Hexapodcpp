#pragma once

#include "../../Component/Software/SoftwareInterface.h"
#include "../Service.h"

namespace Service
{
    namespace General
    {
        using namespace Component::Software;

        class ServiceGeneral : public Service, Event::EventListenerInterface {
        public:
            ServiceGeneral(SoftwareInterface               &software,
                           Message::MessageInterface       &messageListener,
                           Event::EventDispatcherInterface &eventDispatcher);

            ~ServiceGeneral() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         OnEvent(const Event::Event &event) final override;

        protected:
            SoftwareInterface &mSoftware;
        };
    } // namespace General
} // namespace Service

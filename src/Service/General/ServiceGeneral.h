#pragma once

#include "../../Component/Software/SoftwareInterface.h"
#include "../Service.h"

namespace Service
{
    namespace General
    {
        using namespace Component::Software;

        class ServiceGeneral : public Service {
        public:
            ServiceGeneral(SoftwareInterface &software, Event::EventListener &eventListener);

            ~ServiceGeneral() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void DispatchEvent(SEvent &event) final override;

        protected:
            SoftwareInterface &mSoftware;
        };
    }
}

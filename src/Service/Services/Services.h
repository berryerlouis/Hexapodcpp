#pragma once

#include "../Constants.h"
#include "../Service.h"
#include "../Battery/ServiceBattery.h"
#include "../Body/ServiceBody.h"
#include "../Button/ServiceButton.h"
#include "../Communication/ServiceCommunication.h"
#include "../Control/ServiceControl.h"
#include "../Display/ServiceDisplay.h"
#include "../General/ServiceGeneral.h"
#include "../Orientation/ServiceOrientation.h"
#include "../Proximity/ServiceProximity.h"
#include "../Sound/ServiceSound.h"

namespace Service
{
    namespace Services
    {
        using namespace ::Service::General;
        using namespace ::Service::Control;
        using namespace ::Service::Communication;
        using namespace ::Service::Proximity;
        using namespace ::Service::Orientation;
        using namespace ::Service::Battery;
        using namespace ::Service::Display;
        using namespace ::Service::Body;
        using namespace ::Service::Button;
        using namespace ::Service::Sound;

        class Services : public ServiceInterface {
        public:
            Services(ServiceGeneral &serviceGeneral,
                     ServiceControl &serviceControl,
                     ServiceCommunication &serviceCommunication,
                     ServiceProximity &serviceProximity,
                     ServiceOrientation &serviceOrientation,
                     ServiceBattery &serviceBattery,
                     ServiceDisplay &serviceDisplay,
                     ServiceBody &serviceBody,
                     ServiceButton &serviceButton,
                     ServiceSound &serviceSound,
                     Event::MessageInterface &messageListener);

            ~Services() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

        private:
            std::map<EServices, Service *> mServices;
            Event::MessageInterface &mMessageListener;
        };
    }
}
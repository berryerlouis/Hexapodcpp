#pragma once

#include "../Constants.h"
#include "../Service.h"
#include "../Battery/ServiceBattery.h"
#include "../Communication/ServiceCommunication.h"
#include "../Control/ServiceControl.h"
#include "../Display/ServiceDisplay.h"
#include "../General/ServiceGeneral.h"
#include "../Orientation/ServiceOrientation.h"
#include "../Proximity/ServiceProximity.h"

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

        struct ServiceItem {
            EServices serviceId;
            Service *service;

            ServiceItem() = default;

            ~ServiceItem() = default;
        };

        class Services : public ServiceInterface {
        public:
            Services(
                Tick::TickInterface &tick,
                ServiceGeneral &serviceGeneral,
                ServiceControl &serviceControl,
                ServiceCommunication &serviceCommunication,
                ServiceProximity &serviceProximity,
                ServiceOrientation &serviceOrientation,
                ServiceBattery &serviceBattery,
                ServiceDisplay &serviceDisplay,
                Event::EventListener &eventListener);

            ~Services() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

        private:
            void DispatchEvent(void);

            Service *Get(const EServices serviceId);

            Tick::TickInterface &mTick;
            ServiceItem mServices[NB_SERVICES];
            Event::EventListener &mEventListener;
        };
    }
}

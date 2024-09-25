#ifndef GTEST
#include <avr/interrupt.h>
#endif
#include "Services.h"
#include "../../Misc/Logger/Logger.h"

namespace Service
{
    namespace Services
    {
        Services::Services(
            Tick::TickInterface &tick,
            ServiceGeneral &serviceGeneral,
            ServiceControl &serviceControl,
            ServiceCommunication &serviceCommunication,
            ServiceProximity &serviceProximity,
            ServiceOrientation &serviceOrientation,
            ServiceBattery &serviceBattery,
            ServiceDisplay &serviceDisplay,
            Event::EventListenerInterface &eventListener)
            : mTick(tick)
              , mServices{
                  {GENERAL, &serviceGeneral},
                  {PROXIMITY, &serviceProximity},
                  {CONTROL, &serviceControl},
                  {COMMUNICATION, &serviceCommunication},
                  {ORIENTATION, &serviceOrientation},
                  {BATTERY, &serviceBattery},
                  {DISPLAY, &serviceDisplay}
              }
              , mEventListener(eventListener) {
        }

        Core::CoreStatus Services::Initialize(void) {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
            for (const ServiceItem item: this->mServices) {
                success = item.service->Initialize();
                if (!success) {
#ifdef DEBUG
                    const char serviceId[2U] = {static_cast<const char>(item.serviceId + 0x30U),' '};
                    LOG("error");
                    LOG(serviceId);
#endif
#ifndef GTEST
                    sei();
#endif
                }
            }
            return (success);
        }

        void Services::Update(const uint64_t currentTime) {
            (void) currentTime;

            for (const ServiceItem item: this->mServices) {
                const uint64_t currentMillis = this->mTick.GetMs();
                if (item.service->NeedUpdate(currentMillis)) {
                    item.service->Update(currentMillis);
                    item.service->SetNewUpdateTime(currentMillis, item.serviceId);
                }
            }
            this->DispatchEvent();
        }

        Service *Services::Get(const EServices serviceId) {
            for (const ServiceItem &item: this->mServices) {
                if (item.serviceId == serviceId) {
                    return (item.service);
                }
            }
            return (nullptr);
        }

        void Services::DispatchEvent(void) {
            SEvent ev;
            if (this->mEventListener.GetLastEvent(ev)) {
                for (const ServiceItem &item: this->mServices) {
                    item.service->DispatchEvent(ev);
                }
            }
        }
    }
}

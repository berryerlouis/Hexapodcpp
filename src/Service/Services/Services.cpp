#ifdef AVR
#include <avr/interrupt.h>
#endif
#include "../../Misc/Logger/Logger.h"
#include "Services.h"


namespace Service
{
    namespace Services
    {
        Services::Services(
                Tick::TickInterface &tick
                , ServiceGeneral &serviceGeneral
                , ServiceControl &serviceControl
                , ServiceCommunication &serviceCommunication
                , ServiceProximity &serviceProximity
                , ServiceOrientation &serviceOrientation
                , ServiceBattery &serviceBattery
                , ServiceDisplay &serviceDisplay
                , ServiceBody &serviceBody
                , Button::ServiceButton &serviceButton
                , Event::EventListenerInterface &eventListener) :
            mTick(tick)
            , mServices{
                    {GENERAL, &serviceGeneral},
                    {PROXIMITY, &serviceProximity},
                    {CONTROL, &serviceControl},
                    {COMMUNICATION, &serviceCommunication},
                    {ORIENTATION, &serviceOrientation},
                    {BATTERY, &serviceBattery},
                    {DISPLAY, &serviceDisplay},
                    {BODY, &serviceBody},
                    {BUTTON, &serviceButton}},
            mEventListener(eventListener) {
        }

        Core::Status Services::Initialize(void) {
            Core::Status success = Core::Status::CORE_ERROR;

            for (const ServiceItem item: this->mServices) {
                success = item.service->Initialize();
                if (success != Core::Status::CORE_OK) {
#ifdef DEBUG
                    const char serviceId[2U] = {static_cast<const char>(item.serviceId + 0x30U), ' '};
                    LOG("error");
                    LOG(serviceId);
#endif
                }
            }
            return (success);
        }

        void Services::Update(const uint64_t currentTime) {
            static size_t itemIndex = 0U;
            const ServiceItem &item = this->mServices[itemIndex];
            if (item.service->NeedUpdate(currentTime) == Core::Status::CORE_OK) {
                item.service->Update(currentTime);
                item.service->SetNewUpdateTime(this->mTick.GetMs(), item.serviceId);
            }
            if (++itemIndex == NB_SERVICES - 1U) {
                itemIndex = 0U;
            }
        }

        Service *Services::Get(const EServices serviceId) {
            for (const ServiceItem &item: this->mServices) {
                if (item.serviceId == serviceId) {
                    return (item.service);
                }
            }
            return (nullptr);
        }
    } // namespace Services
} // namespace Service

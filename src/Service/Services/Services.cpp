#include "Services.h"
#include "../../Misc/Logger/Logger.h"

namespace Service
{
    namespace Services
    {
        Services::Services(
            ServiceGeneral &serviceGeneral,
            ServiceControl &serviceControl,
            ServiceCommunication &serviceCommunication,
            ServiceProximity &serviceProximity,
            ServiceOrientation &serviceOrientation,
            ServiceBattery &serviceBattery,
            ServiceDisplay &serviceDisplay)
            : mServices{
                {GENERAL, &serviceGeneral},
                {PROXIMITY, &serviceProximity},
                {CONTROL, &serviceControl},
                {COMMUNICATION, &serviceCommunication},
                {ORIENTATION, &serviceOrientation},
                {BATTERY, &serviceBattery},
                {DISPLAY, &serviceDisplay}
            } {
        }

        Core::CoreStatus Services::Initialize(void) {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
            const ServiceCommunication *serviceCom = static_cast<ServiceCommunication *>(this->Get(COMMUNICATION));
            for (const ServiceItem item: this->mServices) {
                item.service->setMediator(serviceCom);
                success = item.service->Initialize();
                if (!success) {
                    LOG("error");
                    LOG(item.serviceId);
                }
            }
            return (success);
        }

        void Services::Update(const uint64_t currentTime) {
            for (const ServiceItem item: this->mServices) {
                if (item.service->NeedUpdate(currentTime)) {
                    item.service->Update(currentTime);
                    item.service->SetNewUpdateTime(currentTime);
                }
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
    }
}

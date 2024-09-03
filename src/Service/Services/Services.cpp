#include "Services.h"
#include "../../Misc/Logger/Logger.h"

namespace Service {
namespace Services {
Services::Services(
        ServiceGeneral &serviceGeneral,
        ServiceControl &serviceControl,
        ServiceCommunication &serviceCommunication,
        ServiceProximity &serviceProximity,
        ServiceOrientation &serviceOrientation,
        ServiceBattery &serviceBattery,
        ServiceDisplay &serviceDisplay)
        : mServices{
        {EServices::GENERAL,       &serviceGeneral},
        {EServices::PROXIMITY,     &serviceProximity},
        {EServices::CONTROL,       &serviceControl},
        {EServices::COMMUNICATION, &serviceCommunication},
        {EServices::ORIENTATION,   &serviceOrientation},
        {EServices::BATTERY,       &serviceBattery},
        {EServices::DISPLAY,       &serviceDisplay}
}
{
}

Core::CoreStatus Services::Initialize ( void )
{
    Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
    ServiceCommunication *serviceCom = (ServiceCommunication *) this->Get(EServices::COMMUNICATION);
    for (ServiceItem item: this->mServices) {
        item.service->setMediator(serviceCom);
        success = item.service->Initialize();
        if (!success) {
            LOG("error");
            LOG(item.serviceId);
        }
    }
    return (success);
}

void Services::Update ( const uint64_t currentTime )
{
    for (ServiceItem item: this->mServices) {
        if (item.service->NeedUpdate(currentTime)) {
            item.service->Update(currentTime);
            item.service->SetNewUpdateTime(currentTime);
        }
    }
}

Service *Services::Get ( const EServices serviceId )
{
    for (ServiceItem &item: this->mServices) {
        if (item.serviceId == serviceId) {
            return (item.service);
        }
    }
    return (nullptr);
}
}
}

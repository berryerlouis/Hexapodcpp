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
                , ServiceButton &serviceButton
                , ServiceSound &serviceSound
                , Event::MessageInterface &messageListener) :
            mTick(tick)
            , mServices{
                    {&serviceGeneral},
                    {&serviceProximity},
                    {&serviceControl},
                    {&serviceCommunication},
                    {&serviceOrientation},
                    {&serviceBattery},
                    {&serviceDisplay},
                    {&serviceBody},
                    {&serviceButton},
                    {&serviceSound}},
            mMessageListener(messageListener) {
        }

        Core::Status Services::Initialize(void) {
            Core::Status success = Core::Status::CORE_ERROR;

            for (Service *service: this->mServices) {
                success = service->Initialize();
                if (success != Core::Status::CORE_OK) {
#ifdef DEBUG
                    const char serviceId[2U] = {static_cast<const char>(item.serviceId + 0x30U), ' '};
                    LOG("error");
                    LOG(serviceId);
#endif
                }
            }
            Frame response;
            Cluster::General::ClusterGeneral::BuildFrameReset(response, success);
            this->mMessageListener.SendMessage(response);
            return (success);
        }

        void Services::Update(const uint64_t currentTime) {
            for (Service *service: this->mServices) {
                service->UpdateService(this->mTick, currentTime);
            }
        }

        Service *Services::Get(const EServices serviceId) {
            for (Service *service: this->mServices) {
                if (service->GetServiceId() == serviceId) {
                    return (service);
                }
            }
            return (nullptr);
        }
    } // namespace Services
} // namespace Service

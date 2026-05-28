#include "Services.h"


namespace Service
{
    namespace Services
    {
        Services::Services(ServiceGeneral            &serviceGeneral,
                           ServiceControl            &serviceControl,
                           ServiceCommunication      &serviceCommunication,
                           ServiceProximity          &serviceProximity,
                           ServiceOrientation        &serviceOrientation,
                           ServiceBattery            &serviceBattery,
                           ServiceDisplay            &serviceDisplay,
                           ServiceBody               &serviceBody,
                           ServiceButton             &serviceButton,
                           ServiceSound              &serviceSound,
                           ServiceGamepad            &serviceGamepad,
                           Message::MessageInterface &messageListener)
            : mServices{{{GENERAL, &serviceGeneral},
                         {CONTROL, &serviceControl},
                         {COMMUNICATION, &serviceCommunication},
                         {PROXIMITY, &serviceProximity},
                         {ORIENTATION, &serviceOrientation},
                         {BATTERY, &serviceBattery},
                         {DISPLAY, &serviceDisplay},
                         {BODY, &serviceBody},
                         {BUTTON, &serviceButton},
                         {SOUND, &serviceSound},
                         {GAMEPAD, &serviceGamepad}}}
            , mMessageListener(messageListener) {
        }

        Core::Status Services::Initialize() {
            Core::Status success = Core::Status::CORE_ERROR;

            for (const auto &pair: this->mServices) {
                Service *service = pair.second;
                success = service->Initialize();
                service->DispatchEvent<Core::Status>(
                        service->GetServiceId(), EventType::EVENT_INIT_UPDATE, success);
                if (success != Core::Status::CORE_OK) {
#ifdef DEBUG
                    LOG_SERVICE_ERROR("Service id:%s Initialization "
                                      "error.",
                                      EServicesStruct::ServiceIdToString(pair.first).c_str());
#endif
                }
            }
            Frame response;
            Cluster::General::ClusterGeneral::BuildFrameReset(response, success);
            this->mMessageListener.SendMessage(response);
            return (success);
        }

        void Services::Update(const uint64_t currentTime) {
            for (const auto &pair: this->mServices) {
                Service *service = pair.second;
                service->UpdateService(currentTime);
            }
        }
    } // namespace Services
} // namespace Service

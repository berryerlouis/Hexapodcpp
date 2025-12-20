#include "Services.h"


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
                ServiceDisplay &serviceDisplay,
                ServiceBody &serviceBody,
                ServiceButton &serviceButton,
                ServiceSound &serviceSound,
                Message::MessageInterface &messageListener,
                Event::EventListenerInterface &eventListener
        ) :
            mServices{
                    {GENERAL, &serviceGeneral},
                    {CONTROL, &serviceControl},
                    {COMMUNICATION, &serviceCommunication},
                    {PROXIMITY, &serviceProximity},
                    {ORIENTATION, &serviceOrientation},
                    {BATTERY, &serviceBattery},
                    {DISPLAY, &serviceDisplay},
                    {BODY, &serviceBody},
                    {BUTTON, &serviceButton},
                    {SOUND, &serviceSound}
            },
            mMessageListener(messageListener),
            mEventListener(eventListener) {
        }

        Core::Status Services::Initialize(void) {
            Core::Status success = Core::Status::CORE_ERROR;

            for (const auto &pair: this->mServices) {
                Service *service = pair.second;
                success = service->Initialize();
                if (success != Core::Status::CORE_OK) {
#ifdef DEBUG
                    LOG_SERVICE_ERROR(
                            "Service id:%s Initialization error.",
                            EServicesStruct::ServiceIdToString(pair.first).c_str()
                    );
#endif
                }
            }
            Frame response;
            Cluster::General::ClusterGeneral::BuildFrameReset(response, success);
            this->mMessageListener.SendMessage(response);
            return (success);
        }

        void Services::Update(const uint64_t currentTime) {
            const Event::EventType event = mEventListener.GetEvent();
            for (const auto &pair: this->mServices) {
                Service *service = pair.second;
                service->UpdateService(currentTime);
                service->DispatchEvent(event);
            }
        }

        /*uint8_t Services::GetEvent(void) {
            uint8_t event = 0U;
            if (!this->mEventList.empty()) {
                event = mEventList.front();
                mEventList.pop_front();
            }
            return event;
        }

        void Services::SetEvent(const uint8_t event) {
            mEventList.push_back(event);
        }

        void Services::UpdateEvent(void) {
            for (const auto &pair: this->mServices) {
                Service *service = pair.second;
                service->DispatchEvent();
            }
        }*/
    } // namespace Services
} // namespace Service

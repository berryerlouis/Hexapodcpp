#include "ServiceGeneral.h"

namespace Service
{
    namespace General
    {
        ServiceGeneral::ServiceGeneral(SoftwareInterface               &software,
                                       Message::MessageInterface       &messageListener,
                                       Event::EventDispatcherInterface &eventDispatcher) :
            Service(GENERAL,
                    10U,
                    messageListener,
                    eventDispatcher),
            mSoftware(software) {
        }

        Core::Status ServiceGeneral::Initialize(void) {
            const Core::Status success = this->mSoftware.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->GetEventDispatcher().AddListener(this);
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceGeneral::Update(const uint64_t currentTime) {
            this->mSoftware.Update(currentTime);
        }

        void ServiceGeneral::OnEvent(const Event::Event &event) {
            if (event.eventType == EventType::EVENT_INIT_UPDATE && event.eventArg.type() == typeid(Core::Status)) {
                if (std::any_cast<Core::Status>(event.eventArg) != Core::Status::CORE_OK) {
                    LOG_SERVICE_ERROR("Service Id %s(%d) Failed on initialization.",
                                      EServicesStruct::ServiceIdToString(event.serviceId).c_str(),
                                      event.serviceId);
                }
            }
        }
    } // namespace General
} // namespace Service

#include "ServiceGeneral.h"
#include "../../Component/Button/Button.h"

namespace Service
{
    namespace General
    {
        ServiceGeneral::ServiceGeneral(
                LedPwmInterface                 &ledPwm,
                SoftwareInterface               &software,
                Message::MessageInterface       &messageListener,
                Event::EventDispatcherInterface &eventDispatcher)
            : Service(GENERAL, 5U, messageListener, eventDispatcher)
            , mLedPwm(ledPwm)
            , mSoftware(software) {
        }

        Core::Status ServiceGeneral::Initialize(void) {
            Core::Status       success = Core::Status::CORE_ERROR;
            const Core::Status success1 = this->mLedPwm.Initialize();
            const Core::Status success2 = this->mSoftware.Initialize();
            if ((Core::Status::CORE_OK == success1) &&
                (Core::Status::CORE_OK == success2)) {
                this->GetEventDispatcher().AddListener(this);
                this->mInitialized = true;
                success = Core::Status::CORE_OK;
            }
            return (success);
        }

        void ServiceGeneral::Update(const uint64_t currentTime) {
            this->mSoftware.Update(currentTime);
            this->mLedPwm.Update(currentTime);
        }

        void ServiceGeneral::OnEvent(const Event::Event &event) {
            if (event.eventType == EventType::EVENT_INIT_UPDATE &&
                event.eventArg.type() == typeid(Core::Status)) {
                if (std::any_cast<Core::Status>(event.eventArg) !=
                    Core::Status::CORE_OK) {
                    LOG_SERVICE_ERROR(
                            "Service Id %s(%d) Failed on "
                            "initialization.",
                            EServicesStruct::ServiceIdToString(event.serviceId)
                                    .c_str(),
                            event.serviceId);
                }
            } else if (event.serviceId == EServices::BUTTON) {
                if (event.eventType == EventType::EVENT_BUTTON_UPDATE) {
                    if (event.eventArg.type() ==
                        typeid(Component::Button::ButtonStruct)) {
                        const Component::Button::ButtonStruct buttonStruct =
                                std::any_cast<Component::Button::ButtonStruct>(
                                        event.eventArg);
                        if (buttonStruct.state ==
                            Component::Button::ButtonState::PUSH) {
                            this->mLedPwm.UpdateFrequency(5.0F);
                        }
                    }
                }
            }
        }


    } // namespace General
} // namespace Service

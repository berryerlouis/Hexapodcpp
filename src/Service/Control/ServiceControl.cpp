#include "ServiceControl.h"

namespace Service
{
    namespace Control
    {
        ServiceControl::ServiceControl(ServosInterface               &servos,
                                       Message::MessageInterface     &messageListener,
                                       Event::EventListenerInterface &eventListener) :
            Service(CONTROL,
                    20U,
                    messageListener,
                    eventListener),
            mStepPca9685(0U),
            mServosInterface(servos) {
        }

        Core::Status ServiceControl::Initialize(void) {
            const Core::Status success = this->mServosInterface.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceControl::Update(const uint64_t currentTime) {
            this->mServosInterface.Update(currentTime);
            this->mServosInterface.GetServosController(this->mStepPca9685).Update(currentTime);
            this->mStepPca9685++;
            if (this->mStepPca9685 == 2U) {
                this->mStepPca9685 = 0U;
            }
        }

        void ServiceControl::DispatchEvent(const Event::Event &event) const {
            (void) event;
        }
    } // namespace Control
} // namespace Service

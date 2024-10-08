#include "ServiceControl.h"

namespace Service
{
    namespace Control
    {
        ServiceControl::ServiceControl(ServosInterface &servos,
                                       Event::EventListenerInterface &eventListener)
            : Service(20U, eventListener)
              , mStepPca9685(0U)
              , mServosInterface(servos) {
        }

        Core::CoreStatus ServiceControl::Initialize(void) {
            return (this->mServosInterface.Initialize());
        }

        void ServiceControl::Update(const uint64_t currentTime) {
            this->mServosInterface.Update(currentTime);
            this->mServosInterface.GetServosController(this->mStepPca9685).Update(currentTime);
            this->mStepPca9685++;
            if (this->mStepPca9685 == 2U) {
                this->mStepPca9685 = 0U;
            }
        }

        void ServiceControl::DispatchEvent(const SEvent &event) {
            (void) event;
        }
    }
}

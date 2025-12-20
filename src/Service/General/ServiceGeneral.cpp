#include "ServiceGeneral.h"

namespace Service
{
    namespace General
    {
        ServiceGeneral::ServiceGeneral(
                SoftwareInterface &software,
                Message::MessageInterface &messageListener,
                Event::EventListenerInterface &eventListener
        ) :
            Service(GENERAL, 1U, messageListener, eventListener), mSoftware(software) {
        }

        Core::Status ServiceGeneral::Initialize(void) {
            const Core::Status success = this->mSoftware.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceGeneral::Update(const uint64_t currentTime) {
            this->mSoftware.Update(currentTime);
        }

        void ServiceGeneral::DispatchEvent(const Event::EventType event) const {
            (void) event;
        }
    } // namespace General
} // namespace Service

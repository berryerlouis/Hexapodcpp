#include "ServiceGeneral.h"

namespace Service
{
    namespace General
    {
        ServiceGeneral::ServiceGeneral(SoftwareInterface &software, Event::EventListenerInterface &eventListener) :
            Service(10000U, eventListener), mSoftware(software) {}

        Core::Status ServiceGeneral::Initialize(void) {
            const Core::Status success = this->mSoftware.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceGeneral::Update(const uint64_t currentTime) { (void) currentTime; }

        void ServiceGeneral::DispatchEvent(const SEvent &event) { (void) event; }
    } // namespace General
} // namespace Service

#include "ServiceGeneral.h"

namespace Service
{
    namespace General
    {
        ServiceGeneral::ServiceGeneral(SoftwareInterface &software,
                                       Event::EventListenerInterface &eventListener)
            : Service(10000U, eventListener)
              , mSoftware(software) {
        }

        Core::CoreStatus ServiceGeneral::Initialize(void) {
            return (this->mSoftware.Initialize());
        }

        void ServiceGeneral::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void ServiceGeneral::DispatchEvent(const SEvent &event) {
            (void) event;
        }
    }
}

#include "ServiceGeneral.h"

namespace Service
{
    namespace General
    {
        ServiceGeneral::ServiceGeneral(SoftwareInterface &software,
                                       Event::EventListener &eventListener)
            : Service(1U, eventListener)
              , mSoftware(software) {
        }

        Core::CoreStatus ServiceGeneral::Initialize(void) {
            return (this->mSoftware.Initialize());
        }

        void ServiceGeneral::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void ServiceGeneral::DispatchEvent(SEvent &event) {
            (void) event;
        }
    }
}

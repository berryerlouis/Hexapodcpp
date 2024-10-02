#include "ServiceOrientation.h"

namespace Service
{
    namespace Orientation
    {
        ServiceOrientation::ServiceOrientation(Mpu9150Interface &imu,
                                               Event::EventListenerInterface &eventListener)
            : Service(10U, eventListener)
              , mImu(imu) {
        }

        Core::CoreStatus ServiceOrientation::Initialize(void) {
            return (this->mImu.Initialize());
        }

        void ServiceOrientation::Update(const uint64_t currentTime) {
            this->mImu.Update(currentTime);
        }

        void ServiceOrientation::DispatchEvent(const SEvent &event) {
            (void) event;
        }
    }
}

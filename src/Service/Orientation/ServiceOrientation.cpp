#include "ServiceOrientation.h"

namespace Service
{
    namespace Orientation
    {
        ServiceOrientation::ServiceOrientation(Mpu9150Interface &imu,
                                               Event::EventListener &eventListener)
            : Service(50U, eventListener)
              , mImu(imu) {
        }

        Core::CoreStatus ServiceOrientation::Initialize(void) {
            return (this->mImu.Initialize());
        }

        void ServiceOrientation::Update(const uint64_t currentTime) {
            this->mImu.Update(currentTime);
        }

        void ServiceOrientation::DispatchEvent(SEvent &event) {
            (void) event;
        }
    }
}

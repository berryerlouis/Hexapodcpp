#include "ServiceOrientation.h"

namespace Service
{
    namespace Orientation
    {
        ServiceOrientation::ServiceOrientation(Mpu9150Interface &imu, BarometerInterface &barometer,
                                               Event::EventListenerInterface &eventListener)
            : Service(10U, eventListener)
              , mImu(imu)
              , mBarometer(barometer) {
        }

        Core::CoreStatus ServiceOrientation::Initialize(void) {
            if (Core::CoreStatus::CORE_OK == mImu.Initialize()) {
                if (Core::CoreStatus::CORE_OK == mBarometer.Initialize()) {
                    return Core::CoreStatus::CORE_OK;
                }
                return Core::CoreStatus::CORE_ERROR;
            }
            return Core::CoreStatus::CORE_ERROR;
        }

        void ServiceOrientation::Update(const uint64_t currentTime) {
            this->mImu.Update(currentTime);
            this->mBarometer.Update(currentTime);
        }

        void ServiceOrientation::DispatchEvent(const SEvent &event) {
            (void) event;
        }
    }
}

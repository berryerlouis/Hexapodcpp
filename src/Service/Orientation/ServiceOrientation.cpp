#include "ServiceOrientation.h"

namespace Service
{
    namespace Orientation
    {
        ServiceOrientation::ServiceOrientation(Mpu9150Interface &imu, BarometerInterface &barometer,
                                               Event::MessageInterface &messageListener) :
            Service(ORIENTATION, 20U, messageListener)
            , mImu(imu)
            , mBarometer(barometer) {
        }

        Core::Status ServiceOrientation::Initialize(void) {
            if (Core::Status::CORE_OK == mImu.Initialize()) {
                if (Core::Status::CORE_OK == mBarometer.Initialize()) {
                    this->mInitialized = true;
                    return Core::Status::CORE_OK;
                }
                return Core::Status::CORE_ERROR;
            }
            return Core::Status::CORE_ERROR;
        }

        void ServiceOrientation::Update(const uint64_t currentTime) {
            this->mImu.Update(currentTime);
            this->mBarometer.Update(currentTime);
        }
    } // namespace Orientation
} // namespace Service
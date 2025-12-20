#include "ServiceOrientation.h"

namespace Service
{
    namespace Orientation
    {
        ServiceOrientation::ServiceOrientation(
                Mpu9150Interface &imu,
                BarometerInterface &barometer,
                Message::MessageInterface &messageListener,
                Event::EventListenerInterface &eventListener
        ) :
            Service(ORIENTATION, 20U, messageListener, eventListener), mImu(imu), mBarometer(barometer) {
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

        void ServiceOrientation::DispatchEvent(const Event::EventType event) const {
            (void) event;
        }
    } // namespace Orientation
} // namespace Service

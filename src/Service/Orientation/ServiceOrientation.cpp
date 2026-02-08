#include "ServiceOrientation.h"

namespace Service
{
    namespace Orientation
    {
        ServiceOrientation::ServiceOrientation(
                Mpu9150Interface                &imu,
                BarometerInterface              &barometer,
                Message::MessageInterface       &messageListener,
                Event::EventDispatcherInterface &eventDispatcher)
            : Service(ORIENTATION, 20U, messageListener, eventDispatcher)
            , mImu(imu)
            , mBarometer(barometer) {
        }

        Core::Status ServiceOrientation::Initialize() {
            const Core::Status successImu = this->mImu.Initialize();
            const Core::Status successBarometer = this->mBarometer.Initialize();
            Core::Status       success = Core::Status::CORE_ERROR;
            if (Core::IsSuccess(successImu) &&
                Core::IsSuccess(successBarometer)) {
                this->GetEventDispatcher().AddListener(this);
                this->mInitialized = true;
                success = Core::CORE_OK;
            }
            return (success);
        }

        void ServiceOrientation::Update(const uint64_t currentTime) {
            this->mImu.Update(currentTime);
            this->mBarometer.Update(currentTime);
        }

        void ServiceOrientation::OnEvent(const Event::Event &event) {
            (void) event;
        }
    } // namespace Orientation
} // namespace Service

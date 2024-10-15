#pragma once

#include "../../Component/Imu/Mpu9150Interface.h"
#include "../../Component/Barometer/Barometer.h"
#include "../Service.h"

namespace Service
{
    namespace Orientation
    {
        using namespace Component::Imu;
        using namespace Component::Barometer;

        class ServiceOrientation : public Service {
        public:
            ServiceOrientation(Mpu9150Interface &imu, BarometerInterface &barometer,
                               Event::EventListenerInterface &eventListener);

            ~ServiceOrientation() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void DispatchEvent(const SEvent &event) final override;

        protected:
            Mpu9150Interface &mImu;
            BarometerInterface &mBarometer;
        };
    }
}

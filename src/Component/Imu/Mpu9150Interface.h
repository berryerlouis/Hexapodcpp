#pragma once

#include "../ComponentInterface.h"
#include "../../Misc/Maths/Geometry.h"

namespace Component
{
    namespace Imu
    {
        using namespace Misc::Maths;

        enum SensorsImu {
            NONE = 0x00,
            ACCEL = 0x01,
            GYRO = 0x02,
            MAG = 0x04,
        };

        class Mpu9150Interface : public ComponentInterface<0U, nullptr_t> {
        public:
            Mpu9150Interface(void) = default;

            ~Mpu9150Interface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual Vector3 ReadAcc(void) const = 0;

            virtual Vector3 ReadGyr(void) const = 0;

            virtual Vector3 ReadMag(void) const = 0;

            virtual int16_t ReadTemp(void) const = 0;

            virtual Imu3d ReadYawPitchRoll(void) = 0;

            virtual void StartCalibration(const SensorsImu sensor) = 0;

            virtual void StopCalibration(const SensorsImu sensor) = 0;
        };
    }
}
#pragma once

#include "../../Driver/Twi/TwiInterface.h"
#include "Mpu9150Interface.h"

namespace Component
{
    namespace Imu
    {
        class Ahrs {
        public:
            Ahrs();

            ~Ahrs() = default;

            void Update(Vector3F acc,
                        Vector3F gyr,
                        Vector3F mag,
                        const float deltaTime);

            void GetRollPitchYaw(Imu3d &ypr) const;

        private:
            float mQuaternion[4U];
            float mGyroMeasError;
            float mBeta;
        };
    }
}


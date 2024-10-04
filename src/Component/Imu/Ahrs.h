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

            void MadgwickQuaternionUpdate(Vector3F &acc,
                                          Vector3F &gyr,
                                          Vector3F &mag,
                                          const float deltaTime);

            void GetYawPitchRoll(Position3D &ypr) const;

        private:
            float mQuaternion[4U];
            float mGyroMeasError;
            float mBeta;
        };
    }
}


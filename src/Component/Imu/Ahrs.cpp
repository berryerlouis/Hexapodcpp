#include "Ahrs.h"
#include "math.h"

namespace Component
{
    namespace Imu
    {
        Ahrs::Ahrs()
            : mQuaternion{1.0F, 0.0F, 0.0F, 0.0F}
              , mGyroMeasError(M_PI * (40.0F / 180.0F))
              , mBeta(sqrt(3.0F / 4.0F) * mGyroMeasError) {
        }

        void Ahrs::GetYawPitchRoll(Position3D &ypr) const {
            ypr.yaw = atan2(
                2.0F * (this->mQuaternion[1] * this->mQuaternion[2] + this->mQuaternion[0] * this->mQuaternion[3]),
                this->mQuaternion[0] * this->mQuaternion[0] + this->mQuaternion[1] * this->mQuaternion[1] - this->
                mQuaternion[2] * this->mQuaternion[2] - this->mQuaternion[3] * this->mQuaternion[3]);
            ypr.pitch = -asin(
                2.0F * (this->mQuaternion[1] * this->mQuaternion[3] - this->mQuaternion[0] * this->mQuaternion[2]));
            ypr.roll = atan2(
                2.0F * (this->mQuaternion[0] * this->mQuaternion[1] + this->mQuaternion[2] * this->mQuaternion[3]),
                this->mQuaternion[0] * this->mQuaternion[0] - this->mQuaternion[1] * this->mQuaternion[1] - this->
                mQuaternion[2] * this->mQuaternion[2] + this->mQuaternion[3] * this->mQuaternion[3]);

            ypr.yaw *= 180.0F / M_PI - 2.46F; // Declination Chirens
            ypr.pitch *= 180.0F / M_PI;
            ypr.roll *= 180.0F / M_PI;
        }

        void Ahrs::MadgwickQuaternionUpdate(Vector3F &acc,
                                            Vector3F &gyr,
                                            Vector3F &mag,
                                            const uint64_t deltaTime) {
            float q1 = this->mQuaternion[0U];
            float q2 = this->mQuaternion[1U];
            float q3 = this->mQuaternion[2U];
            float q4 = this->mQuaternion[3U];
            float norm;
            float hx, hy, _2bx, _2bz;
            float s1, s2, s3, s4;
            float qDot1, qDot2, qDot3, qDot4;

            // Auxiliary variables to avoid repeated arithmetic
            float _2q1mx;
            float _2q1my;
            float _2q1mz;
            float _2q2mx;
            float _4bx;
            float _4bz;
            const float _2q1 = 2.0F * q1;
            const float _2q2 = 2.0F * q2;
            const float _2q3 = 2.0F * q3;
            const float _2q4 = 2.0F * q4;
            const float _2q1q3 = 2.0F * q1 * q3;
            const float _2q3q4 = 2.0F * q3 * q4;
            const float q1q1 = q1 * q1;
            const float q1q2 = q1 * q2;
            const float q1q3 = q1 * q3;
            const float q1q4 = q1 * q4;
            const float q2q2 = q2 * q2;
            const float q2q3 = q2 * q3;
            const float q2q4 = q2 * q4;
            const float q3q3 = q3 * q3;
            const float q3q4 = q3 * q4;
            const float q4q4 = q4 * q4;

            // Normalise accelerometer measurement
            norm = sqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
            if (norm == 0.0F) return; // handle NaN
            norm = 1.0F / norm;
            acc.x *= norm;
            acc.y *= norm;
            acc.z *= norm;

            // Normalise magnetometer measurement
            norm = sqrt(mag.x * mag.x + mag.y * mag.y + mag.z * mag.z);
            if (norm == 0.0F) return; // handle NaN
            norm = 1.0F / norm;
            mag.x *= norm;
            mag.y *= norm;
            mag.z *= norm;

            // Reference direction of Earth's magnetic field
            _2q1mx = 2.0F * q1 * mag.x;
            _2q1my = 2.0F * q1 * mag.y;
            _2q1mz = 2.0F * q1 * mag.z;
            _2q2mx = 2.0F * q2 * mag.x;
            hx = mag.x * q1q1 - _2q1my * q4 + _2q1mz * q3 + mag.x * q2q2 + _2q2 * mag.y * q3 + _2q2 * mag.z * q4 - mag.x
                 * q3q3 - mag.x *
                 q4q4;
            hy = _2q1mx * q4 + mag.y * q1q1 - _2q1mz * q2 + _2q2mx * q3 - mag.y * q2q2 + mag.y * q3q3 + _2q3 * mag.z *
                 q4 - mag.y *
                 q4q4;
            _2bx = sqrt(hx * hx + hy * hy);
            _2bz = -_2q1mx * q3 + _2q1my * q2 + mag.z * q1q1 + _2q2mx * q4 - mag.z * q2q2 + _2q3 * mag.y * q4 - mag.z *
                   q3q3 + mag.z *
                   q4q4;
            _4bx = 2.0F * _2bx;
            _4bz = 2.0F * _2bz;

            // Gradient decent algorithm corrective step
            s1 = -_2q3 * (2.0F * q2q4 - _2q1q3 - acc.x) + _2q2 * (2.0F * q1q2 + _2q3q4 - acc.y) - _2bz * q3 * (
                     _2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mag.x) + (-_2bx * q4 + _2bz * q2) * (
                     _2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - mag.y) + _2bx * q3 * (
                     _2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mag.z);
            s2 = _2q4 * (2.0F * q2q4 - _2q1q3 - acc.x) + _2q1 * (2.0F * q1q2 + _2q3q4 - acc.y) - 4.0F * q2 * (
                     1.0F - 2.0F * q2q2 - 2.0F * q3q3 - acc.z) + _2bz * q4 * (
                     _2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mag.x) + (_2bx * q3 + _2bz * q1) * (
                     _2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - mag.y) + (_2bx * q4 - _4bz * q2) * (
                     _2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mag.z);
            s3 = -_2q1 * (2.0F * q2q4 - _2q1q3 - acc.x) + _2q4 * (2.0F * q1q2 + _2q3q4 - acc.y) - 4.0F * q3 * (
                     1.0F - 2.0F * q2q2 - 2.0F * q3q3 - acc.z) + (-_4bx * q3 - _2bz * q1) * (
                     _2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mag.x) + (_2bx * q2 + _2bz * q4) * (
                     _2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - mag.y) + (_2bx * q1 - _4bz * q3) * (
                     _2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mag.z);
            s4 = _2q2 * (2.0F * q2q4 - _2q1q3 - acc.x) + _2q3 * (2.0F * q1q2 + _2q3q4 - acc.y) + (
                     -_4bx * q4 + _2bz * q2) * (
                     _2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mag.x) + (-_2bx * q1 + _2bz * q3) * (
                     _2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - mag.y) + _2bx * q2 * (
                     _2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mag.z);
            norm = sqrt(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4); // normalise step magnitude
            norm = 1.0F / norm;
            s1 *= norm;
            s2 *= norm;
            s3 *= norm;
            s4 *= norm;

            // Compute rate of change of quaternion
            qDot1 = 0.5f * (-q2 * gyr.x - q3 * gyr.y - q4 * gyr.z) - this->mBeta * s1;
            qDot2 = 0.5f * (q1 * gyr.x + q3 * gyr.z - q4 * gyr.y) - this->mBeta * s2;
            qDot3 = 0.5f * (q1 * gyr.y - q2 * gyr.z + q4 * gyr.x) - this->mBeta * s3;
            qDot4 = 0.5f * (q1 * gyr.z + q2 * gyr.y - q3 * gyr.x) - this->mBeta * s4;

            // Integrate to yield quaternion
            q1 += qDot1 * deltaTime;
            q2 += qDot2 * deltaTime;
            q3 += qDot3 * deltaTime;
            q4 += qDot4 * deltaTime;
            norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4); // normalise quaternion
            norm = 1.0F / norm;
            this->mQuaternion[0U] = q1 * norm;
            this->mQuaternion[1U] = q2 * norm;
            this->mQuaternion[2U] = q3 * norm;
            this->mQuaternion[3U] = q4 * norm;
        }
    }
}

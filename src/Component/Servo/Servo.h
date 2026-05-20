#pragma once
#include "../ServosController/Pca9685.h"
#include "ServoInterface.h"

namespace Component
{
    namespace Servo
    {
        class Servo : public ServoInterface {
            static constexpr uint16_t SERVO_LD_27MG_PWM_MIN_US = 500U;
            static constexpr uint16_t SERVO_LD_27MG_PWM_MAX_US = 2500U;
            static constexpr uint16_t SERVO_LD_27MG_PWM_RANGE_US =
                    SERVO_LD_27MG_PWM_MAX_US - SERVO_LD_27MG_PWM_MIN_US;
            static constexpr uint16_t SERVO_LD_27MG_ANGLE_MIN = 0U;
            static constexpr uint16_t SERVO_LD_27MG_ANGLE_MAX = 270U;
            static constexpr uint16_t SERVO_LD_27MG_ANGLE_MID = SERVO_LD_27MG_ANGLE_MAX / 2U;

            static constexpr uint16_t SERVO_ANGLE_RANGE = 180U;
            static constexpr float    SERVO_LD_27MG_US_PER_DEGREE =
                    (float) ((float) SERVO_LD_27MG_PWM_RANGE_US / (float) SERVO_LD_27MG_ANGLE_MAX);
            static constexpr uint16_t SERVO_ANGLE_MIN =
                    SERVO_LD_27MG_ANGLE_MID - (SERVO_ANGLE_RANGE / 2U);
            static constexpr uint16_t SERVO_ANGLE_MAX =
                    SERVO_LD_27MG_ANGLE_MID + (SERVO_ANGLE_RANGE / 2U);

            static constexpr uint16_t SERVO_PWM_MIN =
                    (SERVO_LD_27MG_PWM_MIN_US + (SERVO_ANGLE_MIN * SERVO_LD_27MG_US_PER_DEGREE)) /
                    ServosController::Pca9685::PCA9685_STEP;
            static constexpr uint16_t SERVO_PWM_MAX =
                    (SERVO_LD_27MG_PWM_MIN_US + (SERVO_ANGLE_MAX * SERVO_LD_27MG_US_PER_DEGREE)) /
                    ServosController::Pca9685::PCA9685_STEP;
            // SERVO_PWM_MIN_US / ServosController::Pca9685::PCA9685_STEP;

            // SERVO_PWM_MAX_US / ServosController::Pca9685::PCA9685_STEP;

        public:
            Servo(ServosController::Pca9685Interface &pca9685,
                  const uint8_t                       servoId,
                  const uint8_t                       angle,
                  const int8_t                        offset,
                  const uint8_t                       min,
                  const uint8_t                       max,
                  const bool                          reverse);

            ~Servo() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual Core::Status SetAngle(const uint8_t  angle,
                                          const uint16_t travelTime) final override;

            virtual uint8_t      GetAngle(void) const final override;

            virtual bool         SetMin(const uint8_t angle) final override;

            virtual uint8_t      GetMin(void) const final override;

            virtual bool         SetMax(const uint8_t angle) final override;

            virtual uint8_t      GetMax(void) const final override;

            virtual bool         SetOffset(const int8_t angle) final override;

            virtual int8_t       GetOffset(void) const final override;

            virtual void         SetReverse(const bool reverse) final override;

            virtual bool         GetReverse(void) final override;

            virtual void         SetEnable(const bool enable) final override;

            virtual bool         IsEnable(void) final override;

            virtual bool         IsMoving(void) final override;

        private:
            uint8_t                             GetAngleFromDeltaTime(const uint64_t currentTime);

            ServosController::Pca9685Interface &mPca9685;
            uint8_t                             mServoId;
            uint8_t                             mAngle;
            uint8_t                             mTargetAngle;
            uint64_t                            mStartTime;
            uint16_t                            mSpeed;
            int8_t                              mOffset;
            uint8_t                             mMin;
            uint8_t                             mMax;
            bool                                mReverse;
            bool                                mEnable;
            bool                                mIsMoving;
        };
    } // namespace Servo
} // namespace Component

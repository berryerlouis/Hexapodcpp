#include "Servo.h"
#include "../../Misc/Maths/Utils.h"

namespace Component
{
    namespace Servo
    {
#define REVERSE_ANGLE( angle )    ( ( ( angle - 90 ) * -1 ) + 90 )

        Servo::Servo(ServosController::Pca9685Interface &pca9685, const uint8_t servoId) :
            mPca9685(pca9685)
            , mServoId(servoId)
            , mAngle(90U)
            , mTargetAngle(0U)
            , mStartTime(0UL)
            , mSpeed(1U)
            , mOffset(0)
            , mMin(SERVO_ANGLE_MIN)
            , mMax(SERVO_ANGLE_MAX)
            , mReverse(false)
            , mEnable(false)
            , mIsMoving(false) {
        }

        Servo::Servo(ServosController::Pca9685Interface &pca9685, const uint8_t servoId,
                     const uint8_t angle) :
            mPca9685(pca9685)
            , mServoId(servoId)
            , mAngle(angle)
            , mTargetAngle(0U)
            , mStartTime(0UL)
            , mSpeed(1U)
            , mOffset(0)
            , mMin(SERVO_ANGLE_MIN)
            , mMax(SERVO_ANGLE_MAX)
            , mReverse(false)
            , mEnable(false)
            , mIsMoving(false) {
        }

        Servo::Servo(ServosController::Pca9685Interface &pca9685, const uint8_t servoId,
                     const uint8_t angle, const int8_t offset) :
            mPca9685(pca9685)
            , mServoId(servoId)
            , mAngle(angle)
            , mTargetAngle(0U)
            , mStartTime(0UL)
            , mSpeed(1U)
            , mOffset(offset)
            , mMin(SERVO_ANGLE_MIN)
            , mMax(SERVO_ANGLE_MAX)
            , mReverse(false)
            , mEnable(false)
            , mIsMoving(false) {
        }

        Servo::Servo(ServosController::Pca9685Interface &pca9685, const uint8_t servoId,
                     const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max) :
            mPca9685(pca9685)
            , mServoId(servoId)
            , mAngle(angle)
            , mTargetAngle(0U)
            , mStartTime(0UL)
            , mSpeed(1U)
            , mOffset(offset)
            , mMin(min)
            , mMax(max)
            , mReverse(false)
            , mEnable(false)
            , mIsMoving(false) {
        }

        Servo::Servo(ServosController::Pca9685Interface &pca9685, const uint8_t servoId,
                     const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max,
                     const bool reverse) :
            mPca9685(pca9685)
            , mServoId(servoId)
            , mAngle(angle)
            , mTargetAngle(0U)
            , mStartTime(0UL)
            , mSpeed(1U)
            , mOffset(offset)
            , mMin(min)
            , mMax(max)
            , mReverse(reverse)
            , mEnable(false)
            , mIsMoving(false) {
        }

        Core::Status Servo::Initialize(void) {
            if (true == this->mReverse) {
                this->mMax = REVERSE_ANGLE(this->mMin);
                this->mMin = REVERSE_ANGLE(this->mMax);
            }
            LOG_COMPONENT_DEBUG("Servo", "id %d on controller address 0x%02X Initialized.", this->mServoId,
                                this->mPca9685.GetAddress());
            return (Core::Status::CORE_OK);
        }

        void Servo::Update(const uint64_t currentTime) {
            if (this->IsMoving()) {
                if(this->mSpeed != 0U) {
                    this->mAngle = this->GetAngleFromDeltaTime(currentTime);
                } else {
                    this->mIsMoving = false;
                }
                if (this->mEnable) {
                    const uint16_t pwm = Misc::Utils::Map(this->mAngle + this->mOffset,
                                                          Servo::SERVO_ANGLE_MIN,
                                                          Servo::SERVO_ANGLE_MAX,
                                                          Servo::SERVO_PWM_MIN,
                                                          Servo::SERVO_PWM_MAX);
                    this->mPca9685.SetPwm(this->mServoId, pwm);
                }
            }
        }

        uint8_t Servo::GetAngleFromDeltaTime(const uint64_t currentTime) {
            const uint64_t endTime = this->mStartTime + this->mSpeed;

            if (currentTime < endTime) {
                float deltaTime = 1.0f;
                deltaTime -= ((endTime - currentTime) / static_cast<float>(this->mSpeed));
                return (Misc::Utils::Lerp<uint8_t>(this->mAngle, this->mTargetAngle, deltaTime));
            }
            this->mIsMoving = false;
            return (this->mTargetAngle);
        }

        bool Servo::IsMoving(void) {
            return (this->mIsMoving);
        }

        Core::Status Servo::SetAngle(const uint8_t angle, const uint16_t travelTime) {
            if (angle < this->mMin) {
                this->mIsMoving = false;
                return (Core::Status::CORE_ERROR_MIN);
            }
            if (angle > this->mMax) {
                this->mIsMoving = false;
                return (Core::Status::CORE_ERROR_MAX);
            }
            if ((angle >= this->mMin) &&
                (angle <= this->mMax)) {
                this->mTargetAngle = angle;
                if (true == this->mReverse) {
                    this->mTargetAngle = REVERSE_ANGLE(angle);
                }
                // Instant move
                if (travelTime == 0U) {
                    this->mAngle = this->mTargetAngle;
                    this->mSpeed = 0U;
                } else {
                    this->mSpeed = travelTime;
                    this->mStartTime = Tick::Tick::GetInstance().GetMs();
                }
                this->mIsMoving = true;
                return (Core::Status::CORE_OK);
            }
            return (Core::Status::CORE_ERROR);
        }

        uint8_t Servo::GetAngle(void) const {
            if (true == this->mReverse) {
                return (REVERSE_ANGLE(this->mAngle));
            }
            return (this->mAngle);
        }

        bool Servo::SetMin(const uint8_t angle) {
            if ((angle <= SERVO_ANGLE_MAX) && (angle <= this->mMax)) {
                this->mMin = angle;
                return (true);
            }
            return (false);
        }

        uint8_t Servo::GetMin(void) const {
            return (this->mMin);
        }

        bool Servo::SetMax(const uint8_t angle) {
            if ((angle <= SERVO_ANGLE_MAX) && (angle >= this->mMin)) {
                this->mMax = angle;
                return (true);
            }
            return (false);
        }

        uint8_t Servo::GetMax(void) const {
            return (this->mMax);
        }

        bool Servo::SetOffset(const int8_t angle) {
            this->mOffset = angle;
            return (true);
        }

        int8_t Servo::GetOffset(void) const {
            return (this->mOffset);
        }

        void Servo::SetReverse(const bool reverse) {
            this->mReverse = reverse;
        }

        bool Servo::GetReverse(void) {
            return (this->mReverse);
        }

        void Servo::SetEnable(const bool enable) {
            this->mEnable = enable;
            if (this->mEnable == true) {
                this->SetAngle(this->mAngle);
            } else {
                this->mPca9685.SetPwm(this->mServoId, 4096U);
            }
        }

        bool Servo::IsEnable(void) {
            return (this->mEnable);
        }
    }
}

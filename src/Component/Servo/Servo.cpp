#include "Servo.h"
#include "../../Misc/Maths/Maths.h"

namespace Component
{
    namespace Servo
    {
#define REVERSE_ANGLE( angle )    ( ( ( angle - 90 ) * -1 ) + 90 )


        Servo::Servo(ServosController::Pca9685Interface &pca9685, Tick::TickInterface &tick, const uint8_t servoId)
            : mPca9685(pca9685)
              , mTick(tick)
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

        Servo::Servo(ServosController::Pca9685Interface &pca9685, Tick::TickInterface &tick, const uint8_t servoId,
                     const uint8_t angle)
            : mPca9685(pca9685)
              , mTick(tick)
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

        Servo::Servo(ServosController::Pca9685Interface &pca9685, Tick::TickInterface &tick, const uint8_t servoId,
                     const uint8_t angle, const int8_t offset)
            : mPca9685(pca9685)
              , mTick(tick)
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

        Servo::Servo(ServosController::Pca9685Interface &pca9685, Tick::TickInterface &tick, const uint8_t servoId,
                     const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max)
            : mPca9685(pca9685)
              , mTick(tick)
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

        Servo::Servo(ServosController::Pca9685Interface &pca9685, Tick::TickInterface &tick, const uint8_t servoId,
                     const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max, const bool reverse)
            : mPca9685(pca9685)
              , mTick(tick)
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

        Core::CoreStatus Servo::Initialize(void) {
            if (true == this->mReverse) {
                this->mMax = REVERSE_ANGLE(this->mMin);
                this->mMin = REVERSE_ANGLE(this->mMax);
            }
            this->mAngle += this->mOffset;
            return (Core::CoreStatus::CORE_OK);
        }

        void Servo::Update(const uint64_t currentTime) {
            if (this->IsMoving()) {
                this->mAngle = this->GetAngleFromDeltaTime(currentTime);
                const uint16_t pwm = Misc::Maths::Map(this->mAngle,
                                                      Servo::SERVO_ANGLE_MIN,
                                                      Servo::SERVO_ANGLE_MAX,
                                                      Servo::SERVO_PWM_MIN,
                                                      Servo::SERVO_PWM_MAX);
                this->mPca9685.SetPwm(this->mServoId, pwm);
            }
        }

        uint8_t Servo::GetAngleFromDeltaTime(const uint64_t currentTime) {
            const uint64_t endTime = this->mStartTime + this->mSpeed;

            if (currentTime < endTime) {
                float deltaTime = 1.0f;
                deltaTime -= ((endTime - currentTime) / static_cast<float>(this->mSpeed));
                return (Misc::Maths::Lerp(this->mAngle, this->mTargetAngle + this->mOffset, deltaTime));
            }
            this->mIsMoving = false;
            return (this->mTargetAngle + this->mOffset);
        }

        bool Servo::IsMoving(void) {
            return (this->mIsMoving);
        }

        Core::CoreStatus Servo::SetAngle(const uint8_t angle, const uint16_t travelTime) {
            if ((true == this->mEnable) &&
                (angle >= this->mMin + this->mOffset) &&
                (angle <= this->mMax + this->mOffset)) {
                this->mTargetAngle = angle;
                if (true == this->mReverse) {
                    this->mTargetAngle = REVERSE_ANGLE(angle);
                }
                // Instant move
                if ((travelTime == 0U) &&
                    (static_cast<int8_t>(this->mTargetAngle + this->mOffset) >= 0)) {
                    this->mAngle = this->mTargetAngle + this->mOffset;
                    this->mIsMoving = true;
                } else {
                    this->mIsMoving = true;
                    this->mSpeed = travelTime;
                    this->mStartTime = this->mTick.GetMs();
                }
                return (Core::CoreStatus::CORE_OK);
            }
            this->mIsMoving = false;
            return (Core::CoreStatus::CORE_ERROR);
        }

        uint8_t Servo::GetAngle(void) const {
            return (this->mAngle - this->mOffset);
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
            this->mIsMoving = true;
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
        }

        bool Servo::IsEnable(void) {
            return (this->mEnable);
        }
    }
}

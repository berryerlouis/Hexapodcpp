#include "math.h"
#include "Servo.h"

#define REVERSE_ANGLE(angle)    ( ( (angle - 90) * -1) + 90)

Servo::Servo(Pca9685 &pca9685, const uint8_t servoId)
	: mPca9685(pca9685)
	, mServoId(servoId)
	, mAngle(90)
	, mTargetAngle(0)
	, mStartTime(0)
	, mSpeed(1)
	, mOffset(0)
	, mMin(SERVO_ANGLE_MIN)
	, mMax(SERVO_ANGLE_MAX)
	, mReverse(false)
	, mEnable(true)
	, mIsMoving(false)
{
}

Servo::Servo(Pca9685 &pca9685, const uint8_t servoId, const uint8_t angle)
	: mPca9685(pca9685)
	, mServoId(servoId)
	, mAngle(angle)
	, mTargetAngle(0)
	, mStartTime(0)
	, mSpeed(1)
	, mOffset(0)
	, mMin(SERVO_ANGLE_MIN)
	, mMax(SERVO_ANGLE_MAX)
	, mReverse(false)
	, mEnable(true)
	, mIsMoving(false)
{
}

Servo::Servo(Pca9685 &pca9685, const uint8_t servoId, const uint8_t angle, const int8_t offset)
	: mPca9685(pca9685)
	, mServoId(servoId)
	, mAngle(angle)
	, mTargetAngle(0)
	, mStartTime(0)
	, mSpeed(1)
	, mOffset(offset)
	, mMin(SERVO_ANGLE_MIN)
	, mMax(SERVO_ANGLE_MAX)
	, mReverse(false)
	, mEnable(true)
	, mIsMoving(false)
{
}

Servo::Servo(Pca9685 &pca9685, const uint8_t servoId, const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max)
	: mPca9685(pca9685)
	, mServoId(servoId)
	, mAngle(angle)
	, mTargetAngle(0)
	, mStartTime(0)
	, mSpeed(1)
	, mOffset(offset)
	, mMin(min)
	, mMax(max)
	, mReverse(false)
	, mEnable(true)
	, mIsMoving(false)
{
}

Servo::Servo(Pca9685 &pca9685, const uint8_t servoId, const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max, const bool reverse)
	: mPca9685(pca9685)
	, mServoId(servoId)
	, mAngle(angle)
	, mTargetAngle(0)
	, mStartTime(0)
	, mSpeed(1)
	, mOffset(offset)
	, mMin(min)
	, mMax(max)
	, mReverse(reverse)
	, mEnable(true)
	, mIsMoving(false)
{
}

void Servo::Initialize (void)
{
	if (true == this->mReverse)
	{
		this->mMax = REVERSE_ANGLE(this->mMin);
		this->mMin = REVERSE_ANGLE(this->mMax);
	}
	this->SetAngle(this->mAngle);
}

void Servo::Update (const uint64_t currentTime)
{
	if (this->IsMoving() )
	{
		this->mAngle = this->GetAngleFromDeltaTime(currentTime);
		uint16_t pwm = map(this->mAngle, Servo::SERVO_ANGLE_MIN, Servo::SERVO_ANGLE_MAX, Servo::SERVO_PWM_MIN, Servo::SERVO_PWM_MAX);
		this->mPca9685.SetPwm(this->mServoId, pwm);
	}
}

uint8_t Servo::GetAngleFromDeltaTime (const uint64_t currentTime)
{
	const uint64_t endTime   = this->mStartTime + this->mSpeed;
	float          deltaTime = 1.0f;

	if (currentTime < endTime)
	{
		deltaTime -= ( (endTime - currentTime) / (float) (this->mSpeed) );
		return (this->Lerp(this->mAngle, this->mTargetAngle + this->mOffset, deltaTime) );
	}
	this->mIsMoving = false;
	return (this->mTargetAngle + this->mOffset);
}

uint8_t Servo::Lerp (uint8_t a, uint8_t b, float t)
{
	return ( (uint8_t) ( (float) a + (float) ( (b - a) * t) ) );
}

bool Servo::IsMoving (void)
{
	return (this->mIsMoving);
}

bool Servo::SetAngle (const uint8_t angle, const uint16_t travelTime)
{
	if ( (true == this->mEnable) &&
		  (angle >= this->mMin) &&
		  (angle <= this->mMax) )
	{
		this->mIsMoving    = false;
		this->mSpeed       = travelTime;
		this->mTargetAngle = angle;
		//this->mStartTime   = millis();

		if (true == this->mReverse)
		{
			this->mTargetAngle = REVERSE_ANGLE(angle);
		}

		if (travelTime == 0)
		{
			this->mAngle = this->mTargetAngle + this->mOffset;
		}
		this->mIsMoving = true;
		return (true);
	}
	return (false);
}

uint8_t Servo::GetAngle (void) const
{
	return (this->mAngle - this->mOffset);
}

bool Servo::SetMin (const uint8_t angle)
{
	if ( (angle <= SERVO_ANGLE_MAX) && (angle <= this->mMax) )
	{
		this->mMin = angle;
		return (true);
	}
	return (false);
}

uint8_t Servo::GetMin (void) const
{
	return (this->mMin);
}

bool Servo::SetMax (const uint8_t angle)
{
	if ( (angle <= SERVO_ANGLE_MAX) && (angle >= this->mMin) )
	{
		this->mMax = angle;
		return (true);
	}
	return (false);
}

uint8_t Servo::GetMax (void) const
{
	return (this->mMax);
}

bool Servo::SetOffset (const int8_t angle)
{
	this->mOffset   = angle;
	this->mIsMoving = true;
	return (true);
}

int8_t Servo::GetOffset (void) const
{
	return (this->mOffset);
}

void Servo::SetReverse (const bool reverse)
{
	this->mReverse = reverse;
}

bool Servo::GetReverse (void)
{
	return (this->mReverse);
}

void Servo::SetEnable (const bool enable)
{
	this->mEnable = enable;
}

bool Servo::IsEnable (void)
{
	return (this->mEnable);
}

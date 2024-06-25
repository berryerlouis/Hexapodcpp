#pragma once
#include "Pca9685Interface.h"
#include "ServoInterface.h"
#include "../drv/TickInterface.h"

namespace Component {
using namespace Driver;
class Servo : public ServoInterface {
	static const uint16_t SERVO_PWM_MIN     = 168U;
	static const uint16_t SERVO_PWM_MAX     = 413U;
	static const uint16_t SERVO_ANGLE_MIN   = 0U;
	static const uint16_t SERVO_ANGLE_MAX   = 180;
	static const uint16_t SERVO_ANGLE_RANGE = 180;

public:
	Servo( Pca9685Interface &pca9685, TickInterface &tick, const uint8_t servoId );
	Servo( Pca9685Interface &pca9685, TickInterface &tick, const uint8_t servoId, const uint8_t angle );
	Servo( Pca9685Interface &pca9685, TickInterface &tick, const uint8_t servoId, const uint8_t angle, const int8_t offset );
	Servo( Pca9685Interface &pca9685, TickInterface &tick, const uint8_t servoId, const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max );
	Servo( Pca9685Interface &pca9685, TickInterface &tick, const uint8_t servoId, const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max, const bool reverse );
	~Servo() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual Core::CoreStatus SetAngle( const uint8_t angle, const uint16_t travelTime = 0U ) final override;
	virtual uint8_t GetAngle( void ) const final override;
	virtual bool SetMin( const uint8_t angle ) final override;
	virtual uint8_t GetMin( void ) const final override;
	virtual bool SetMax( const uint8_t angle ) final override;
	virtual uint8_t GetMax( void ) const final override;
	virtual bool SetOffset( const int8_t angle ) final override;
	virtual int8_t GetOffset( void ) const final override;
	virtual void SetReverse( const bool reverse ) final override;
	virtual bool GetReverse( void ) final override;
	virtual void SetEnable( const bool enable ) final override;
	virtual bool IsEnable( void ) final override;
	virtual bool IsMoving( void ) final override;

private:
	uint8_t GetAngleFromDeltaTime( const uint64_t currentTime );
	uint8_t Lerp( uint8_t a, uint8_t b, float t );

	inline long map ( long x, long in_min, long in_max, long out_min, long out_max )
	{
		return ( ( x - in_min ) * ( out_max - out_min ) / ( in_max - in_min ) + out_min );
	}

	Pca9685Interface &mPca9685;
	TickInterface &mtick;
	uint8_t mServoId;
	uint8_t mAngle;
	uint8_t mTargetAngle;
	uint64_t mStartTime;
	uint16_t mSpeed;
	int8_t mOffset;
	uint8_t mMin;
	uint8_t mMax;
	bool mReverse;
	bool mEnable;
	bool mIsMoving;
};
}

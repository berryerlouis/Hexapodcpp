#include "Pca9685.h"
namespace Component {
Pca9685::Pca9685( TwiInterface &i2c, const uint8_t address )
	: mI2c( i2c )
	, mAddress( address )
	, mInternalOscillatorFrequency( EConstant::FREQUENCY_OSCILLATOR )
	, mPwm{ 0U, 0U }
{
}

Core::CoreStatus Pca9685::Initialize ( void )
{
	this->Reset();
	return ( Core::CoreStatus::CORE_OK );
}

void Pca9685::Update ( const uint64_t currentTime )
{
	(void) currentTime;
	this->mI2c.WriteRegisters( this->mAddress, (uint8_t) ERegister::LED0_ON_L, (uint8_t *) this->mPwm, 4 * EConstant::NB_LEDS );
}

void Pca9685::Reset ( void )
{
	this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::MODE1, (uint8_t) ERegisterMode1::RESTART );

	for ( size_t idPwm = 0U; idPwm < EConstant::NB_LEDS; idPwm++ )
	{
		this->SetPwm( idPwm, EConstant::LED_OFF );
	}

	this->SetFrequency( SERVO_FREQUENCY );
}

void Pca9685::Sleep ( void )
{
	uint8_t awake = 0U;

	this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::MODE1, awake );
	awake |= ERegisterMode1::SLEEP;
	this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::MODE1, awake );
}

void Pca9685::WakeUp ( void )
{
	uint8_t wakeUp = 0U;

	this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::MODE1, wakeUp );
	wakeUp &= ~ERegisterMode1::SLEEP;
	this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::MODE1, wakeUp );
}

void Pca9685::setOscillatorFrequency ( const uint32_t frequency )
{
	this->mInternalOscillatorFrequency = frequency;
}

void Pca9685::SetFrequency ( const uint32_t frequency )
{
	float prescaleval = ( ( mInternalOscillatorFrequency / ( frequency * 4096.0 ) ) + 0.5 ) - 1.0;

	if ( prescaleval < EConstant::PRESCALE_MIN )
	{
		prescaleval = EConstant::PRESCALE_MIN;
	}

	if ( prescaleval > EConstant::PRESCALE_MAX )
	{
		prescaleval = EConstant::PRESCALE_MAX;
	}
	uint8_t prescale = (uint8_t) prescaleval;

	uint8_t oldmode = 0U;
	this->mI2c.ReadRegister( this->mAddress, (uint8_t) ERegister::MODE1, oldmode );
	uint8_t newmode = ( oldmode & ~ERegisterMode1::RESTART ) | ERegisterMode1::SLEEP;
	this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::MODE1, newmode );
	this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::PRESCALE, prescale );
	this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::MODE1, oldmode );
	this->mI2c.WriteRegister( this->mAddress, (uint8_t) ERegister::MODE1, (uint8_t) ( oldmode | ERegisterMode1::RESTART | ERegisterMode1::AI ) );
}

void Pca9685::SetPwm ( uint8_t num, uint16_t off )
{
	this->mPwm[num].on = 0U;

	if ( off > 100 && off < 500 )
	{
		this->mPwm[num].off = off;
		return;
	}
	this->mPwm[num].off = EConstant::LED_OFF;
	return;
}
}

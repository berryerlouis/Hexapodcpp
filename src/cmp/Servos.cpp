#include "Servos.h"

namespace Component {
Servos::Servos( Pca9685Interface &pca9685_0, Pca9685Interface &pca9685_1, TickInterface &tick )
	: mServo0( pca9685_0, tick, 0, 90U, 15, 60U, 120U, false )
	, mServo1( pca9685_0, tick, 1, 90U, -4, 60U, 120U, false )
	, mServo2( pca9685_0, tick, 2, 90U, 12, 60U, 120U, false )
	, mServo3( pca9685_0, tick, 3, 90U, 2, 60U, 120U, false )
	, mServo4( pca9685_0, tick, 4, 90U, 0, 60U, 120U, false )
	, mServo5( pca9685_0, tick, 5, 90U, 18, 60U, 120U, false )
	, mServo6( pca9685_0, tick, 6, 90U, 2, 60U, 120U, false )
	, mServo7( pca9685_0, tick, 7, 90U, 0, 60U, 120U, false )
	, mServo8( pca9685_0, tick, 8, 90U, 10, 60U, 120U, false )
	, mServo9( pca9685_1, tick, 0, 90U, -20, 60U, 120U, true )
	, mServo10( pca9685_1, tick, 1, 90U, -2, 60U, 120U, true )
	, mServo11( pca9685_1, tick, 2, 90U, -15, 60U, 120U, true )
	, mServo12( pca9685_1, tick, 3, 90U, 0, 60U, 120U, true )
	, mServo13( pca9685_1, tick, 4, 90U, 0, 60U, 120U, true )
	, mServo14( pca9685_1, tick, 5, 90U, -20, 60U, 120U, true )
	, mServo15( pca9685_1, tick, 6, 90U, 10, 60U, 120U, true )
	, mServo16( pca9685_1, tick, 7, 90U, 0, 60U, 120U, true )
	, mServo17( pca9685_1, tick, 8, 90U, -18, 60U, 120U, true )
	, mServos{ &mServo0, &mServo1, &mServo2, &mServo3, &mServo4, &mServo5, &mServo6, &mServo7, &mServo8,
				 &mServo9, &mServo10, &mServo11, &mServo12, &mServo13, &mServo14, &mServo15, &mServo16, &mServo17 }
	, mPca9685Left( pca9685_0 )
	, mPca9685Right( pca9685_1 )
{
}

Core::CoreStatus Servos::Initialize ( void )
{
	uint32_t success = 0UL;
	this->mPca9685Left.Initialize();
	this->mPca9685Right.Initialize();

	for ( size_t servoId = 0U; servoId < NB_SERVOS; servoId++ )
	{
		success |= this->mServos[servoId]->Initialize() << servoId;
	}

	return ( ( success == 0x3FFFFUL ) ? Core::CoreStatus::CORE_OK : Core::CoreStatus::CORE_ERROR );
}

void Servos::Update ( const uint32_t currentTime )
{
	for ( Servo *servo : this->mServos )
	{
		servo->Update( currentTime );
	}
}

Servo &Servos::GetServo ( const uint8_t servoId )
{
	return ( *this->mServos[servoId] );
}

Pca9685Interface &Servos::GetPca9685 ( const uint8_t pca9685Id )
{
	if ( pca9685Id == 0U )
	{
		return ( this->mPca9685Left );
	}
	return ( this->mPca9685Right );
}
}

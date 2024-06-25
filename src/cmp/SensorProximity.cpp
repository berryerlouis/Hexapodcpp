#include "SensorProximity.h"

namespace Component {
SensorProximity::SensorProximity( ProximityInterface &srf05Left, ProximityInterface &srf05Right, ProximityInterface &Vl53l0x )
	: mSrf05Left( srf05Left )
	, mSrf05Right( srf05Right )
	, mVl53l0x( Vl53l0x )
	, mStepSrf( 0U )
	, mStepVlx( 0U )
{
}

Core::CoreStatus SensorProximity::Initialize ( void )
{
	uint8_t success = 0U;

	this->mStepSrf = 0U;
	this->mStepVlx = 0U;
	success        = this->mSrf05Left.Initialize() << 0U;
	success       |= this->mSrf05Right.Initialize() << 1U;
	success       |= this->mVl53l0x.Initialize() << 2U;

	return ( ( success == 7U ) ? Core::CoreStatus::CORE_OK : Core::CoreStatus::CORE_ERROR );
}

void SensorProximity::Update ( const uint64_t currentTime )
{
	if ( this->mStepSrf == 0 )
	{
		this->mSrf05Left.Update( currentTime );
		this->mStepSrf = SensorProximity::UPDATE_STEP_SRF_MS;
	}
	else if ( this->mStepSrf == SensorProximity::UPDATE_STEP_SRF_MS )
	{
		this->mSrf05Right.Update( currentTime );
		this->mStepSrf = 0U;
	}

	if ( ++this->mStepVlx == SensorProximity::UPDATE_STEP_VLX_MS )
	{
		this->mVl53l0x.Update( currentTime );
		this->mStepVlx = 0U;
	}
}

bool SensorProximity::IsDetecting ( const SensorsId sensorId )
{
	bool success = false;

	switch ( sensorId )
	{
	case SensorsId::SRF_LEFT:
		success = this->mSrf05Left.IsDetecting();
		break;

	case SensorsId::SRF_RIGHT:
		success = this->mSrf05Right.IsDetecting();
		break;

	case SensorsId::VLX:
		success = this->mVl53l0x.IsDetecting();
		break;
	}
	return ( success );
}

uint16_t SensorProximity::GetDistance ( const SensorsId sensorId )
{
	uint16_t distance = 0U;

	switch ( sensorId )
	{
	case SensorsId::SRF_LEFT:
		distance = this->mSrf05Left.GetDistance();
		break;

	case SensorsId::SRF_RIGHT:
		distance = this->mSrf05Right.GetDistance();
		break;

	case SensorsId::VLX:
		distance = this->mVl53l0x.GetDistance();
		break;
	}
	return ( distance );
}

Core::CoreStatus SensorProximity::SetThreshold ( const SensorsId sensorId, const uint16_t threshold )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	switch ( sensorId )
	{
	case SensorsId::SRF_LEFT:
		success = this->mSrf05Left.SetThreshold( threshold );
		break;

	case SensorsId::SRF_RIGHT:
		success = this->mSrf05Right.SetThreshold( threshold );
		break;

	case SensorsId::VLX:
		success = this->mVl53l0x.SetThreshold( threshold );
		break;
	}
	return ( success );
}

uint16_t SensorProximity::GetThreshold ( const SensorsId sensorId )
{
	uint16_t threshold = 0U;

	switch ( sensorId )
	{
	case SensorsId::SRF_LEFT:
		threshold = this->mSrf05Left.GetThreshold();
		break;

	case SensorsId::SRF_RIGHT:
		threshold = this->mSrf05Right.GetThreshold();
		break;

	case SensorsId::VLX:
		threshold = this->mVl53l0x.GetThreshold();
		break;
	}
	return ( threshold );
}
}

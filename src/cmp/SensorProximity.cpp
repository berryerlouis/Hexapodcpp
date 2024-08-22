#include "SensorProximity.h"

namespace Component {
SensorProximity::SensorProximity( SensorProximityInterface &srf05Left, SensorProximityInterface &srf05Right, SensorProximityInterface &Vl53l0x )
	: mSensors{ &srf05Left, &srf05Right, &Vl53l0x }
{
}

Core::CoreStatus SensorProximity::Initialize ( void )
{
	uint8_t success = 0U;

	for ( size_t sensorId = 0; sensorId < NB_SENSORS; sensorId++ )
	{
		if ( this->mSensors[sensorId]->Initialize() )
		{
			this->mSensors[sensorId]->Attach( this );
			success |= 1U << sensorId;
		}
	}

	return ( ( success == 7U ) ? Core::CoreStatus::CORE_OK : Core::CoreStatus::CORE_ERROR );
}

void SensorProximity::Update ( const uint64_t currentTime )
{
	for ( size_t sensorId = 0; sensorId < NB_SENSORS; sensorId++ )
	{
		this->mSensors[sensorId]->Update( currentTime );
	}
}

uint16_t SensorProximity::GetDistance ( const SensorsId &sensorId )
{
	return ( this->mSensors[sensorId]->GetDistance() );
}

Core::CoreStatus SensorProximity::SetThreshold ( const SensorsId &sensorId, const uint16_t threshold )
{
	return ( this->mSensors[sensorId]->SetThreshold( threshold ) );
}

uint16_t SensorProximity::GetThreshold ( const SensorsId &sensorId )
{
	return ( this->mSensors[sensorId]->GetThreshold() );
}

void SensorProximity::Detect ( const SensorsId &sensorId, const uint16_t &distance )
{
	this->Notify( sensorId, distance );
}
}

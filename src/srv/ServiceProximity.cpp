#include "ServiceProximity.h"

<<<<<<< HEAD
ServiceProximity::ServiceProximity( ClusterProximity &clusterProximity, SensorProximityMultipleInterface &proximity )
=======
ServiceProximity::ServiceProximity( ClusterProximity &clusterProximity, SensorProximityInterface &proximity )
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
	: Service( 100 )
	, mClusterProximity( clusterProximity )
	, mProximity( proximity )
{
}

Core::CoreStatus ServiceProximity::Initialize ( void )
{
<<<<<<< HEAD
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	if ( this->mProximity.Initialize() )
	{
		this->mProximity.Attach( this );
		success = Core::CoreStatus::CORE_OK;
	}
	return ( success );
=======
	return ( this->mProximity.Initialize() );
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
}

void ServiceProximity::Update ( const uint64_t currentTime )
{
	this->mProximity.Update( currentTime );
<<<<<<< HEAD
}

void ServiceProximity::Detect ( const SensorsId &sensorId, const uint16_t &distance )
{
	(void) distance;
	Frame response;
	this->mClusterProximity.BuildFrameDistance( (EProximityCommands) sensorId, response );
	this->mServiceMediator->SendFrame( response );
=======

	for ( size_t sensorId = 0U; sensorId < SensorProximityInterface::NB_SENSORS; sensorId++ )
	{
		if ( this->mProximity.IsDetecting( (SensorProximityInterface::SensorsId) sensorId ) )
		{
			Frame response;
			this->mClusterProximity.BuildFrameDistance( (EProximityCommands) sensorId, response );
			this->mServiceMediator->SendFrame( response );
			this->mServiceMediator->DisplayProximitySensor( (SensorProximityInterface::SensorsId) sensorId );
		}
	}
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
}

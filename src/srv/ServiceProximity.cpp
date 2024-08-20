#include "ServiceProximity.h"

ServiceProximity::ServiceProximity( ClusterProximity &clusterProximity, SensorProximityInterface &proximity )
	: Service( 100 )
	, mClusterProximity( clusterProximity )
	, mProximity( proximity )
{
}

Core::CoreStatus ServiceProximity::Initialize ( void )
{
	return ( this->mProximity.Initialize() );
}

void ServiceProximity::Update ( const uint64_t currentTime )
{
	this->mProximity.Update( currentTime );

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
}

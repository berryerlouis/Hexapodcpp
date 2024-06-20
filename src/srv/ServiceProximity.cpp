#include "ServiceProximity.h"

ServiceProximity::ServiceProximity( ClusterProximity &clusterProximity )
	: Service( 100 )
	, mClusterProximity( clusterProximity )
{
}

Core::CoreStatus ServiceProximity::Initialize ( void )
{
	return ( this->mClusterProximity.Initialize() );
}

void ServiceProximity::Update ( const uint32_t currentTime )
{
	this->mClusterProximity.Update( currentTime );

	for ( size_t sensorId = 0U; sensorId < SensorProximityInterface::NB_SENSORS; sensorId++ )
	{
		if ( this->mClusterProximity.GetComponent().IsDetecting( (SensorProximityInterface::SensorsId) sensorId ) )
		{
			Frame response;
			this->mClusterProximity.BuildFrameDistance( (EProximityCommands) sensorId, response );
			this->mServiceMediator->SendFrame( response );
		}
	}
}

#include "ServiceProximity.h"

namespace Service {
namespace Proximity {
ServiceProximity::ServiceProximity( ClusterProximity &clusterProximity, SensorProximityMultipleInterface &proximity )
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

void ServiceProximity::Detect ( const SensorsId &sensorId, const uint16_t &distance )
{
	(void) distance;
	Frame response;
	this->mClusterProximity.BuildFrameDistance( (Cluster::EProximityCommands) sensorId, response );
	this->mServiceMediator->SendFrame( response );
}
}
}

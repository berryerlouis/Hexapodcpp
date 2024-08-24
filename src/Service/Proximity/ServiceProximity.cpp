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
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	if ( this->mProximity.Initialize() )
	{
		this->mProximity.Attach( this );
		success = Core::CoreStatus::CORE_OK;
	}
	return ( success );
}

void ServiceProximity::Update ( const uint64_t currentTime )
{
	this->mProximity.Update( currentTime );
}

void ServiceProximity::Detect ( const SensorsId &sensorId )
{
	Frame response;
	this->mClusterProximity.BuildFrameDistance( (Cluster::EProximityCommands) sensorId, response );
	this->mServiceMediator->SendFrame( response );
}
}
}

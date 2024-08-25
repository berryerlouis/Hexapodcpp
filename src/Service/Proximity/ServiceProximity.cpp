#include "ServiceProximity.h"

namespace Service {
namespace Proximity {
ServiceProximity::ServiceProximity( SensorProximityMultipleInterface &proximity )
	: Service( 10 )
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
	this->mMediator->Notify( { id: Cluster::EClusters::PROXIMITY, value: sensorId } );
}
}
}

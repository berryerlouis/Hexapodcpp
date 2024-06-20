#include "ServiceOrientation.h"

ServiceOrientation::ServiceOrientation( ClusterImu &clusterImu )
	: Service( 50 )
	, mClusterImu( clusterImu )
{
}

Core::CoreStatus ServiceOrientation::Initialize ( void )
{
	return ( this->mClusterImu.Initialize() );
}

void ServiceOrientation::Update ( const uint32_t currentTime )
{
	this->mClusterImu.Update( currentTime );
}

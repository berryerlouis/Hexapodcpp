#include "ServiceOrientation.h"

ServiceOrientation::ServiceOrientation( ClusterImu &clusterImu )
	: Service( 50U )
	, mClusterImu( clusterImu )
{
}

Core::CoreStatus ServiceOrientation::Initialize ( void )
{
	return ( this->mClusterImu.Initialize() );
}

void ServiceOrientation::Update ( const uint64_t currentTime )
{
	this->mClusterImu.Update( currentTime );
}

#include "ServiceOrientation.h"

ServiceOrientation::ServiceOrientation( ClusterImu &clusterImu, Mpu9150Interface &imu )
	: Service( 50U )
	, mClusterImu( clusterImu )
	, mImu( imu )
{
}

Core::CoreStatus ServiceOrientation::Initialize ( void )
{
	return ( this->mImu.Initialize() );
}

void ServiceOrientation::Update ( const uint64_t currentTime )
{
	this->mImu.Update( currentTime );
}

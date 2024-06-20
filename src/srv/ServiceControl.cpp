#include "ServiceControl.h"

ServiceControl::ServiceControl( ClusterServo &clusterServo )
	: Service( 10U )
	, mClusterServo( clusterServo )
	, mStepPca9685( 0U )
{
}

Core::CoreStatus ServiceControl::Initialize ( void )
{
	return ( this->mClusterServo.Initialize() );
}

void ServiceControl::Update ( const uint32_t currentTime )
{
	this->mClusterServo.Update( currentTime );
	this->mClusterServo.GetComponent().GetPca9685( this->mStepPca9685 ).Update( currentTime );
	this->mStepPca9685++;
	if ( this->mStepPca9685 == 2U )
	{
		this->mStepPca9685 = 0U;
	}
}

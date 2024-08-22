#include "ServiceControl.h"

ServiceControl::ServiceControl( ClusterServo &clusterServo, ServosInterface &servos )
	: Service( 10U )
	, mClusterServo( clusterServo )
	, mStepPca9685( 0U )
	, mServosInterface( servos )
{
}

Core::CoreStatus ServiceControl::Initialize ( void )
{
	return ( this->mServosInterface.Initialize() );
}

void ServiceControl::Update ( const uint64_t currentTime )
{
	this->mServosInterface.Update( currentTime );
	this->mServosInterface.GetPca9685( this->mStepPca9685 ).Update( currentTime );
	this->mStepPca9685++;
	if ( this->mStepPca9685 == 2U )
	{
		this->mStepPca9685 = 0U;
	}
}

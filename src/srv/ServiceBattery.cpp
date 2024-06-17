#include "ServiceBattery.h"

ServiceBattery::ServiceBattery( Clusters::ClusterBattery &clusterBattery )
	: Service( 100 )
	, mClusterBattery( clusterBattery )
	, mCurrentState( BatteryInterface::BatteryState::UNKNOWN )
{
}

Core::CoreStatus ServiceBattery::Initialize ( void )
{
	return ( this->mClusterBattery.Initialize() );
}

void ServiceBattery::Update ( const uint32_t currentTime )
{
	this->mClusterBattery.Update( currentTime );
	auto state = this->mClusterBattery.GetComponent().GetState();
	if ( this->mCurrentState != state )
	{
		this->mCurrentState = state;
		Frame response;
		this->mClusterBattery.BuildFrameState( response );
		this->mServiceMediator->SendFrame( response );
	}
}

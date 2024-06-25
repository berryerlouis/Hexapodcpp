#include "ServiceBattery.h"

ServiceBattery::ServiceBattery( Clusters::ClusterBattery &clusterBattery )
	: Service( 100U )
	, mClusterBattery( clusterBattery )
	, mCurrentState( BatteryInterface::BatteryState::UNKNOWN )
{
}

Core::CoreStatus ServiceBattery::Initialize ( void )
{
	return ( this->mClusterBattery.Initialize() );
}

void ServiceBattery::Update ( const uint64_t currentTime )
{
	this->mClusterBattery.Update( currentTime );
	auto state = this->mClusterBattery.GetComponent().GetState();
	if ( this->mCurrentState != state )
	{
		Frame response;
		this->mCurrentState = state;
		this->mClusterBattery.BuildFrameState( response );
		this->mServiceMediator->SendFrame( response );
		this->mServiceMediator->DisplayBatteryLevel( state );
	}
}

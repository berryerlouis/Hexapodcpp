#include "ServiceGeneral.h"

ServiceGeneral::ServiceGeneral( ClusterGeneral &clusterGeneral )
	: Service( 1000U )
	, mClusterGeneral( clusterGeneral )
{
}

Core::CoreStatus ServiceGeneral::Initialize ( void )
{
	return ( this->mClusterGeneral.Initialize() );
}

void ServiceGeneral::Update ( const uint32_t currentTime )
{
	SoftwareInterface & software = this->mClusterGeneral.GetComponent();
	if ( currentTime < software.GetMinTime() )
	{
		Frame response;
		software.SetMinTime( currentTime );
		this->mClusterGeneral.BuildFrameGetMinTime( response );
		this->mServiceMediator->SendFrame( response );
	}
	else if ( currentTime > software.GetMaxTime() )
	{
		Frame response;
		software.SetMaxTime( currentTime );
		this->mClusterGeneral.BuildFrameGetMaxTime( response );
		this->mServiceMediator->SendFrame( response );
	}
}

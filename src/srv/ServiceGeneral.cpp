#include "ServiceGeneral.h"

ServiceGeneral::ServiceGeneral( ClusterGeneral &clusterGeneral )
	: Service( 1U )
	, mClusterGeneral( clusterGeneral )
{
}

Core::CoreStatus ServiceGeneral::Initialize ( void )
{
	return ( this->mClusterGeneral.Initialize() );
}

void ServiceGeneral::Update ( const uint64_t currentTime )
{
	if ( this->GetPreviousTime() > 0U )
	{
		SoftwareInterface & software = this->mClusterGeneral.GetComponent();
		uint64_t            delta    = currentTime - this->GetPreviousTime();
		if ( delta < software.GetMinTime() )
		{
			Frame response;
			software.SetMinTime( delta );
			this->mClusterGeneral.BuildFrameGetMinTime( response );
			this->mServiceMediator->SendFrame( response );
		}
		else if ( delta > software.GetMaxTime() )
		{
			Frame response;
			software.SetMaxTime( delta );
			this->mClusterGeneral.BuildFrameGetMaxTime( response );
			this->mServiceMediator->SendFrame( response );
		}
	}
}

#include "ServiceGeneral.h"
#include "../cor/Log.h"

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
	const uint64_t previousTime = this->GetPreviousTime();
	if ( previousTime > 0U )
	{
		SoftwareInterface & software = this->mClusterGeneral.GetComponent();
		uint64_t            delta    = currentTime - previousTime;

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

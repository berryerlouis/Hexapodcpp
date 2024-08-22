#include "ServiceGeneral.h"
#include "../../Misc/Logger/Logger.h"

namespace Service {
namespace General {
ServiceGeneral::ServiceGeneral( ClusterGeneral &clusterGeneral, SoftwareInterface &software )
	: Service( 1U )
	, mClusterGeneral( clusterGeneral )
	, mSoftware( software )
{
}

Core::CoreStatus ServiceGeneral::Initialize ( void )
{
	return this->mSoftware.Initialize();;
}

void ServiceGeneral::Update ( const uint64_t currentTime )
{
	const uint64_t previousTime = this->GetPreviousTime();
	uint64_t       delta        = currentTime - previousTime;

	if ( delta < this->mSoftware.GetMinTime() )
	{
		Frame response;
		this->mSoftware.SetMinTime( delta );
		this->mClusterGeneral.BuildFrameGetMinTime( response );
		this->mServiceMediator->SendFrame( response );
	}
	else if ( delta > this->mSoftware.GetMaxTime() )
	{
		Frame response;
		this->mSoftware.SetMaxTime( delta );
		this->mClusterGeneral.BuildFrameGetMaxTime( response );
		this->mServiceMediator->SendFrame( response );
	}
}
}
}

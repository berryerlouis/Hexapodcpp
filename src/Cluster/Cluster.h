#pragma once

#include "ClusterInterface.h"

namespace Cluster {
class Cluster : public ClusterInterface {
public:
	Cluster( EClusters clusterId )
		: mClusterId( clusterId )
	{
	}

	virtual Core::CoreStatus Execute( Frame &request, Frame &response ) = 0;

	virtual EClusters GetId ( void )
	{
		return ( mClusterId );
	}

protected:
	const EClusters mClusterId;
};
}

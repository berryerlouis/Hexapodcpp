#pragma once

#include "../clu/ClusterGeneral.h"
#include "ServiceCommon.h"

class ServiceGeneral : public ServiceCommon <ClusterGeneral> {
public:
	ServiceGeneral( ClusterGeneral &clusterGeneral ) :
		ServiceCommon <ClusterGeneral>( 1000U, clusterGeneral )
	{
	}

	~ServiceGeneral() = default;
};

#pragma once

#include "Constants.h"
#include "Frame.h"
#include "ClusterInterface.h"

namespace Clusters {
class ClustersInterface {
public:
	ClustersInterface()  = default;
	~ClustersInterface() = default;

	virtual ClusterInterface *GetCluster( const EClusters clusterId ) const = 0;
};
}

#pragma once

#include "Cluster.h"

namespace Cluster {
class ClustersInterface {
public:
	ClustersInterface()  = default;
	~ClustersInterface() = default;

	virtual Cluster *GetCluster(const uint8_t clusterId) const = 0;
};
}

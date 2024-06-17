#pragma once

#include "ClusterBattery.h"
#include "ClusterGeneral.h"
#include "ClusterBody.h"
#include "ClusterImu.h"
#include "ClusterProximity.h"
#include "ClusterServo.h"
#include "ClustersInterface.h"

namespace Clusters {
using namespace Component;
class Clusters : public ClustersInterface {
public:
	Clusters(
		ClusterGeneral &general,
		ClusterBattery &battery,
		ClusterBody &body,
		ClusterImu &imu,
		ClusterProximity &proximity,
		ClusterServo &servo );
	~Clusters() = default;

	virtual ClusterInterface *GetCluster( const uint8_t clusterId ) const final override;

private:
	ClusterInterface *mClusters[NB_CLUSTERS];
};
}

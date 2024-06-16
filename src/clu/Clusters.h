#pragma once

#include "Cluster.h"
#include "ClusterBattery.h"
#include "ClusterBody.h"
#include "ClusterGeneral.h"
#include "ClusterImu.h"
#include "ClusterProximity.h"
#include "ClusterServo.h"
#include "Constants.h"
#include "ClustersInterface.h"

namespace Cluster {
using namespace Component;
class Clusters : public ClustersInterface {
public:
	Clusters(Battery &battery, Mpu9150 &mpu9150, SensorProximity &sensorProximity, Servos &servos, Body &body);
	~Clusters() = default;

	virtual Cluster *GetCluster(const uint8_t clusterId) const final override;

private:
	ClusterGeneral mClusterGeneral;
	ClusterBattery mClusterBattery;
	ClusterImu mClusterImu;
	ClusterProximity mClusterProximity;
	ClusterServo mClusterServo;
	ClusterBody mClusterBody;

	Cluster *mClusters[NB_CLUSTERS];
};
}

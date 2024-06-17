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
	Clusters(BatteryInterface &battery, Mpu9150Interface &mpu9150, SensorProximity &sensorProximity, ServosInterface &servos, Body &body);
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

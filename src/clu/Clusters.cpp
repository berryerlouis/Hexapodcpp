#include "Clusters.h"

namespace Cluster {
using namespace Component;
Clusters::Clusters(BatteryInterface &battery, Mpu9150Interface &mpu9150, SensorProximity &sensorProximity, ServosInterface &servos, Body &body)
	: mClusterGeneral()
	, mClusterBattery(battery)
	, mClusterImu(mpu9150)
	, mClusterProximity(sensorProximity)
	, mClusterServo(servos)
	, mClusterBody(body)
	, mClusters{&mClusterGeneral, &mClusterImu, &mClusterProximity, &mClusterServo, &mClusterBattery, &mClusterBody}
{
}

Cluster *Clusters::GetCluster (const uint8_t clusterId) const
{
	if (clusterId < NB_CLUSTERS)
	{
		return (this->mClusters[clusterId]);
	}
	return (nullptr);
}
}

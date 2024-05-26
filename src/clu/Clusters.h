#pragma once

#include "Constants.h"
#include "Cluster.h"
#include "ClusterGeneral.h"
#include "ClusterBattery.h"
#include "ClusterImu.h"
#include "ClusterProximity.h"
#include "ClusterServo.h"
#include "ClusterBody.h"

class Clusters
{
public:
  Clusters(Battery &battery, Mpu9150 &mpu9150, SensorProximity &sensorProximity, Servos &servos, Body &body);
  ~Clusters() = default;

  Cluster *GetCluster(const uint8_t clusterId) const;

private:
  ClusterGeneral mClusterGeneral;
  ClusterBattery mClusterBattery;
  ClusterImu mClusterImu;
  ClusterProximity mClusterProximity;
  ClusterServo mClusterServo;
  ClusterBody mClusterBody;

  Cluster *mClusters[NB_CLUSTERS];
};
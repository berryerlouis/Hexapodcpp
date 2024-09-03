#pragma once

#include "../Battery/ClusterBattery.h"
#include "../General/ClusterGeneral.h"
#include "../Body/ClusterBody.h"
#include "../Imu/ClusterImu.h"
#include "../Proximity/ClusterProximity.h"
#include "../Servo/ClusterServo.h"
#include "ClustersInterface.h"

namespace Cluster {
namespace Clusters {
using namespace ::Cluster::General;
using namespace ::Cluster::Body;
using namespace ::Cluster::Proximity;
using namespace ::Cluster::Imu;
using namespace ::Cluster::Battery;
using namespace ::Cluster::Servo;

class Clusters : public ClustersInterface
{
public:
    Clusters(
        ClusterGeneral &general,
        ClusterBattery &battery,
        ClusterBody &body,
        ClusterImu &imu,
        ClusterProximity &proximity,
        ClusterServo &servo );
    ~Clusters() = default;

    virtual ClusterInterface *GetCluster( const EClusters clusterId ) const final override;

private:
    ClusterInterface *mClusters[NB_CLUSTERS];
};
}
}

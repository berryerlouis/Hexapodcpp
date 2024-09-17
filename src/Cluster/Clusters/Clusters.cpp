#include "Clusters.h"

namespace Cluster
{
    namespace Clusters
    {
        Clusters::Clusters(
            ClusterGeneral &general,
            ClusterBattery &battery,
            ClusterBody &body,
            ClusterImu &imu,
            ClusterProximity &proximity,
            ClusterServo &servo)
            : mClusters{&general, &battery, &body, &imu, &proximity, &servo} {
        }

        ClusterInterface *Clusters::GetCluster(const EClusters clusterId) const {
            for (ClusterInterface *cluster: mClusters) {
                if (cluster->GetId() == clusterId) {
                    return (cluster);
                }
            }
            return (nullptr);
        }
    }
}

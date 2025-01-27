#include "Clusters.h"

namespace Cluster
{
    namespace Clusters
    {
        Clusters::Clusters(
                ClusterGeneral &general,
                ClusterBattery &battery,
                ClusterButton &button,
                ClusterBody &body,
                ClusterImu &imu,
                ClusterProximity &proximity,
                ClusterServo &servo) :
            mClusters{&general, &battery, &body, &imu, &proximity, &servo, &button} {
        }

        ClusterInterface *Clusters::GetCluster(const EClusters clusterId) const {
            for (ClusterInterface *cluster: mClusters) {
                if (cluster != nullptr && cluster->GetId() == clusterId) {
                    return (cluster);
                }
            }
            return (nullptr);
        }
    }
}

#include "Clusters.h"

namespace Cluster
{
    namespace Clusters
    {
        Clusters::Clusters(
                ClusterGeneral &general,
                ClusterBattery &battery,
                ClusterButton &button,
                ClusterSound &sound,
                ClusterBody &body,
                ClusterImu &imu,
                ClusterProximity &proximity,
                ClusterServo &servo) :
            mClusters{&general, &battery, &body, &imu, &proximity, &servo, &button, &sound} {
        }

        ClusterInterface *Clusters::GetCluster(const EClusters clusterId) const {
            for (ClusterInterface *cluster: mClusters) {
                if (cluster != nullptr && cluster->GetClusterId() == clusterId) {
                    return (cluster);
                }
            }
            return (nullptr);
        }
    }
}

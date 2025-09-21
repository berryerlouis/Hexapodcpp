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
            mClusters{
                    {GENERAL, &general},
                    {BATTERY, &battery},
                    {BUTTON, &button},
                    {SOUND, &sound},
                    {BODY, &body},
                    {IMU, &imu},
                    {PROXIMITY, &proximity},
                    {SERVO, &servo}
            } {
            LOG_CLUSTER_DEBUG("Clusters", " Initialized.");
        }

        ClusterBase *Clusters::GetCluster(const EClusters clusterId) {
            const auto it = mClusters.find(clusterId);
            if (it != mClusters.end()) {
                return it->second;
            }
            return nullptr;
        }
    }
}

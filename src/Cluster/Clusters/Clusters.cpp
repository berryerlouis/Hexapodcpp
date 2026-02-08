#include "Clusters.h"

namespace Cluster
{
    namespace Clusters
    {
        Clusters::Clusters(ClusterGeneral   &general,
                           ClusterBattery   &battery,
                           ClusterButton    &button,
                           ClusterSound     &sound,
                           ClusterBody      &body,
                           ClusterImu       &imu,
                           ClusterProximity &proximity,
                           ClusterServo     &servo)
            : mClusters{{GENERAL, std::ref(general)},
                        {BATTERY, std::ref(battery)},
                        {BUTTON, std::ref(button)},
                        {SOUND, std::ref(sound)},
                        {BODY, std::ref(body)},
                        {IMU, std::ref(imu)},
                        {PROXIMITY, std::ref(proximity)},
                        {SERVO, std::ref(servo)}} {
            LOG_CLUSTER_DEBUG("Clusters", " Initialized.");
        }

        ClusterBase *Clusters::GetCluster(const EClusters clusterId) {
            const auto cluster = mClusters.find(clusterId);
            if (cluster != mClusters.end()) {
                return &(cluster->second.get());
            }
            return nullptr;
        }
    } // namespace Clusters
} // namespace Cluster

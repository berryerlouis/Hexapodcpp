#pragma once

#include <functional>
#include <map>

#include "../Battery/ClusterBattery.h"
#include "../Body/ClusterBody.h"
#include "../Button/ClusterButton.h"
#include "../General/ClusterGeneral.h"
#include "../Imu/ClusterImu.h"
#include "../Proximity/ClusterProximity.h"
#include "../Servo/ClusterServo.h"
#include "../Sound/ClusterSound.h"
#include "ClustersInterface.h"

namespace Cluster
{
    namespace Clusters
    {
        using namespace ::Cluster::General;
        using namespace ::Cluster::Body;
        using namespace ::Cluster::Proximity;
        using namespace ::Cluster::Imu;
        using namespace ::Cluster::Battery;
        using namespace ::Cluster::Button;
        using namespace ::Cluster::Sound;
        using namespace ::Cluster::Servo;

        class Clusters : public ClustersInterface {
        public:
            Clusters(ClusterGeneral   &general,
                     ClusterBattery   &battery,
                     ClusterButton    &button,
                     ClusterSound     &sound,
                     ClusterBody      &body,
                     ClusterImu       &imu,
                     ClusterProximity &proximity,
                     ClusterServo     &servo);

            ~Clusters() = default;

            ClusterBase *GetCluster(const EClusters clusterId) final override;

        private:
            std::map<EClusters, std::reference_wrapper<ClusterBase>> mClusters;
        };
    } // namespace Clusters
} // namespace Cluster

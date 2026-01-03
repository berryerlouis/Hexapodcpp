#pragma once

#include "../ClusterBase.h"
#include "../Constants.h"

namespace Cluster
{
    namespace Clusters
    {
        class ClustersInterface {
        public:
            ClustersInterface() = default;

            virtual ClusterBase *GetCluster(const EClusters clusterId) = 0;
        };
    } // namespace Clusters
} // namespace Cluster

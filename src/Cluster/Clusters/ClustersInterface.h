#pragma once

#include "../Constants.h"
#include "../ClusterInterface.h"

namespace Cluster
{
    namespace Clusters
    {
        class ClustersInterface {
        public:
            ClustersInterface() = default;

            ~ClustersInterface() = default;

            virtual ClusterInterface *GetCluster(const EClusters clusterId) const = 0;
        };
    }
}

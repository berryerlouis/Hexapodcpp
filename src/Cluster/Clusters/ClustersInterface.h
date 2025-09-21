#pragma once

#include "../Constants.h"
#include "../ClusterBase.h"

namespace Cluster
{
    namespace Clusters
    {
        class ClustersInterface {
        public:
            ClustersInterface() = default;

            ~ClustersInterface() = default;

            virtual ClusterBase *GetCluster(const EClusters clusterId) = 0;
        };
    }
}

#pragma once
#include <gmock/gmock.h>
#include "../../../src/Cluster/Clusters/ClustersInterface.h"

namespace Cluster
{
    namespace Clusters
    {
        class MockClusters : public ClustersInterface {
        public:
            MOCK_METHOD1(GetCluster, ClusterBase *(const EClusters));
        };
    }
}
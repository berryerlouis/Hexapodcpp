#pragma once
#include <gmock/gmock.h>
#include "../../../src/Cluster/Clusters/ClustersInterface.h"

namespace Cluster
{
	namespace Clusters
	{
		class MockClusters : public ClustersInterface {
		public:
			MOCK_CONST_METHOD1(GetCluster, ClusterInterface * ( const EClusters ));
		};
	}
}

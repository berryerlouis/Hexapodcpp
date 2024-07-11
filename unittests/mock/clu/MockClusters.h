#pragma once
#include <gmock/gmock.h>
#include "../../../src/clu/ClustersInterface.h"

namespace Component {
class MockClusters : public Clusters::ClustersInterface {
public:
	MOCK_CONST_METHOD1( GetCluster, Clusters::ClusterInterface *( const Clusters::EClusters ) );
};
}

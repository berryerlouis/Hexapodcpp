#pragma once
#include <gmock/gmock.h>
#include "../../../src/clu/ClustersInterface.h"

namespace Component {
class MockClusters : public Cluster::ClustersInterface {
public:
	MOCK_CONST_METHOD1(GetCluster, Cluster::Cluster *(const uint8_t) );
};
}

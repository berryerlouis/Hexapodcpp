#pragma once
#include <gmock/gmock.h>
#include "../../../src/Cluster/Battery/ClusterBattery.h"
#include "../../../src/Cluster/Clusters/ClustersInterface.h"
#include "../cmp/MockBattery.h"

namespace Cluster {
namespace Battery {
using namespace Component::Battery;

class MockClusterBattery : public ClusterBattery {
public:
	MockClusterBattery( Component::Battery::MockBattery &mockBattery )
		: ClusterBattery( mockBattery )
	{
	}

	~MockClusterBattery() = default;
};
}
}

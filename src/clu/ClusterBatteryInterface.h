#pragma once

#include "Cluster.h"
#include "../cmp/BatteryInterface.h"

namespace Clusters {
using namespace Component;

class ClusterBatteryInterface : public ClusterInterface {
public:
	ClusterBatteryInterface()  = default;
	~ClusterBatteryInterface() = default;

	virtual Core::CoreStatus Execute( Frame &request, Frame &response ) = 0;
};
}

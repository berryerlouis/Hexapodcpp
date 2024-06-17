#pragma once

#include "Constants.h"
#include "Frame.h"
namespace Cluster {
class ClusterInterface {
public:
	ClusterInterface()  = default;
	~ClusterInterface() = default;

	virtual Core::CoreStatus Execute(Frame &request, Frame &response) = 0;
};
}

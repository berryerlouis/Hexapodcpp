#pragma once

#include "Constants.h"
#include "Frame/Frame.h"

namespace Cluster {
class ClusterInterface
{
public:
	ClusterInterface()  = default;
	~ClusterInterface() = default;

	virtual Core::CoreStatus Execute( Frame &request, Frame &response ) = 0;
	virtual EClusters GetId( void ) = 0;
};
}

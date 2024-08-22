#pragma once

#include "Constants.h"
#include "Frame.h"
namespace Clusters {
class ClusterInterface {
public:
	ClusterInterface()  = default;
	~ClusterInterface() = default;

	virtual Core::CoreStatus Execute( Frame &request, Frame &response ) = 0;
	virtual EClusters GetId( void ) = 0;
};
}

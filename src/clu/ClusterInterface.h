#pragma once

#include "Constants.h"
#include "Frame.h"
namespace Clusters {
class ClusterInterface {
public:
	ClusterInterface()  = default;
	~ClusterInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	virtual Core::CoreStatus Execute( Frame &request, Frame &response ) = 0;
	virtual EClusters GetId( void ) = 0;
};
}

#pragma once

#include "../clu/ClusterProximity.h"
#include "Service.h"


using namespace Component;
class ServiceProximity : public Service {
public:
	ServiceProximity( ClusterProximity &clusterProximity );
	~ServiceProximity() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

protected:
	ClusterProximity &mClusterProximity;
};

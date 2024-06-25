#pragma once

#include "../clu/ClusterImu.h"
#include "Service.h"


using namespace Component;

class ServiceOrientation : public Service {
public:
	ServiceOrientation( ClusterImu &clusterImu );
	~ServiceOrientation() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

protected:
	ClusterImu &mClusterImu;
};

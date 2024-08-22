#pragma once

#include "../cmp/Mpu9150Interface.h"
#include "../clu/ClusterImu.h"
#include "Service.h"


using namespace Component;

class ServiceOrientation : public Service {
public:
	ServiceOrientation( ClusterImu &clusterImu, Mpu9150Interface &imu );
	~ServiceOrientation() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

protected:
	ClusterImu &mClusterImu;
	Mpu9150Interface &mImu;
};

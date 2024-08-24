#pragma once

#include "../../Component/Imu/Mpu9150Interface.h"
#include "../../Cluster/Imu/ClusterImu.h"
#include "../Service.h"

namespace Service {
namespace Orientation {
using namespace Component::Imu;
using namespace Cluster::Imu;

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
}
}

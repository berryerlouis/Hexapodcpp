#pragma once

#include "../../Component/Battery/BatteryInterface.h"
#include "../../Component/Servos/ServosInterface.h"
#include "../../Cluster/Servo/ClusterServo.h"
#include "../Service.h"

namespace Service {
namespace Control {
using namespace Component::Servos;
using namespace Cluster::Servo;

class ServiceControl : public Service {
public:
	ServiceControl( ClusterServo &clusterServo, ServosInterface &servos );
	~ServiceControl() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

protected:
	ClusterServo &mClusterServo;
	uint8_t mStepPca9685;
	ServosInterface &mServosInterface;
};
}
}

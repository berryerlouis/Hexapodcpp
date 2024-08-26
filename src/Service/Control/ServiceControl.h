#pragma once

#include "../../Component/Servos/ServosInterface.h"
#include "../Service.h"

namespace Service {
namespace Control {
using namespace Component::Servos;

class ServiceControl : public Service {
public:
	ServiceControl( ServosInterface &servos );
	~ServiceControl() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

protected:
	uint8_t mStepPca9685;
	ServosInterface &mServosInterface;
};
}
}

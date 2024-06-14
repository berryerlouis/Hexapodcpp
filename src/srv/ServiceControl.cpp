#include "ServiceControl.h"

ServiceControl::ServiceControl(Servos &servos)
	: Service(10U)
	, mServos(servos)
	, mStepPca9685(0U)
{
}

bool ServiceControl::Initialize (void)
{
	return (this->mServos.Initialize() );
}

void ServiceControl::Update (const uint32_t currentTime)
{
	this->mServos.Update(currentTime);
	this->mServos.GetPca9685(this->mStepPca9685).Update(currentTime);
	this->mStepPca9685++;

	if (this->mStepPca9685 == 2U)
	{
		this->mStepPca9685 = 0U;
	}
}

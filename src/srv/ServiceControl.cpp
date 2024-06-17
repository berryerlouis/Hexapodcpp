#include "ServiceControl.h"

ServiceControl::ServiceControl(ServosInterface &servos)
	: Service(10U)
	, mServos(servos)
	, mStepPca9685(0U)
{
}

Core::CoreStatus ServiceControl::Initialize (void)
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

#include "Battery.h"

namespace Component {
#define NOMINAL_LEVEL    80U
#define WARNING_LEVEL    75U

Battery::Battery(AdcInterface &adc)
	: mVoltage(0U)
	, mState(BatteryState::UNKNOWN)
	, mAdc(adc)
{
}

bool Battery::Initialize (void)
{
	return (this->mAdc.Initialize() );
}

void Battery::Update (const uint32_t currentTime)
{
	(void) currentTime;
	this->mVoltage = this->mAdc.Read();

	if (this->mVoltage >= NOMINAL_LEVEL)
	{
		this->mState = NOMINAL;
	}
	else if (this->mVoltage >= WARNING_LEVEL)
	{
		this->mState = WARNING;
	}
	else
	{
		this->mState = CRITICAL;
	}
}

Battery::BatteryState Battery::GetState (void)
{
	return (this->mState);
}

uint16_t Battery::GetVoltage (void)
{
	return (this->mVoltage);
}
}

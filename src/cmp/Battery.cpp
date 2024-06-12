#include "Battery.h"

namespace Component {
#define NOMINAL_LEVEL    80U
#define WARNING_LEVEL    75U

Battery::Battery(const SGpio &gpio)
	: mVoltage(0U)
	, mState(BatteryState::UNKNOWN)
	, mAdc(gpio)
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

bool Battery::BuildFrameVoltage (Frame &response)
{
	uint16_t volt     = GetVoltage();
	uint8_t  params[] = { (uint8_t) (volt >> 8U), (uint8_t) volt };

	return (response.Build(
				  EClusters::BATTERY,
				  EBatteryCommands::GET_VOLTAGE,
				  params,
				  2U) );
}

bool Battery::BuildFrameState (Frame &response)
{
	uint16_t volt     = GetVoltage();
	uint8_t  params[] = { GetState(), (uint8_t) (volt >> 8U), (uint8_t) volt };

	return (response.Build(
				  EClusters::BATTERY,
				  EBatteryCommands::GET_BAT_STATUS,
				  params,
				  3U) );
}
}

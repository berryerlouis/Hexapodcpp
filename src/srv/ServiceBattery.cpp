#include "ServiceBattery.h"

ServiceBattery::ServiceBattery(BatteryInterface &battery)
	: Service(100)
	, mBattery(battery)
	, mCurrentState(BatteryInterface::BatteryState::UNKNOWN)
{
}

Core::CoreStatus ServiceBattery::Initialize (void)
{
	return (this->mBattery.Initialize() );
}

void ServiceBattery::Update (const uint32_t currentTime)
{
	this->mBattery.Update(currentTime);
	auto state = this->mBattery.GetState();
	if (this->mCurrentState != state)
	{
		this->mCurrentState = state;
		Frame response;
		this->BuildFrameState(response);
		this->mServiceMediator->SendFrame(response);
	}
}

Core::CoreStatus ServiceBattery::BuildFrameState (Frame &response)
{
	uint16_t volt     = this->mBattery.GetVoltage();
	uint8_t  params[] = { this->mCurrentState, (uint8_t) (volt >> 8U), (uint8_t) volt };

	return (response.Build(
				  EClusters::BATTERY,
				  EBatteryCommands::GET_BAT_STATUS,
				  params,
				  3U) );
}

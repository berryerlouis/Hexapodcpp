#include "ServiceBattery.h"

ServiceBattery::ServiceBattery(Battery &battery)
	: Service(100)
	, mBattery(battery)
{
}

bool ServiceBattery::Initialize (void)
{
	return (this->mBattery.Initialize() );
}

void ServiceBattery::Update (const uint32_t currentTime)
{
	auto state = this->mBattery.GetState();

	this->mBattery.Update(currentTime);
	if (state != this->mBattery.GetState() )
	{
		Frame response;
		this->BuildFrameState(response);
		this->mServiceMediator->SendFrame(response);
	}
}

bool ServiceBattery::BuildFrameState (Frame &response)
{
	uint16_t volt     = this->mBattery.GetVoltage();
	uint8_t  params[] = { this->mBattery.GetState(), (uint8_t) (volt >> 8U), (uint8_t) volt };

	return (response.Build(
				  EClusters::BATTERY,
				  EBatteryCommands::GET_BAT_STATUS,
				  params,
				  3U) );
}

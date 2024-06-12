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
		this->mBattery.BuildFrameState(response);
		this->mServiceMediator->SendFrame(response);
	}
}

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
	this->mBattery.Update(currentTime);
}

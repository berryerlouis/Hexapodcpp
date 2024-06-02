#include "ServiceBattery.h"

ServiceBattery::ServiceBattery(Battery&battery)
	: Service(20)
	, mBattery(battery) {
}

void ServiceBattery::Initialize (void) {
	this->mBattery.Initialize();
}

void ServiceBattery::Update (const uint32_t currentTime) {
	this->mBattery.Update(currentTime);
}

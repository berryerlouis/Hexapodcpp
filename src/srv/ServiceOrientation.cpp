#include "ServiceOrientation.h"

ServiceOrientation::ServiceOrientation(Mpu9150Interface &mpu9150)
	: Service(20)
	, mMpu9150(mpu9150)
{
}

Core::CoreStatus ServiceOrientation::Initialize (void)
{
	return (this->mMpu9150.Initialize() );
}

void ServiceOrientation::Update (const uint32_t currentTime)
{
	this->mMpu9150.Update(currentTime);
}

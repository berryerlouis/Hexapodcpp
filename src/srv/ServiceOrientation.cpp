#include "ServiceOrientation.h"

ServiceOrientation::ServiceOrientation(Mpu9150 &mpu9150) : Service(20), mMpu9150(mpu9150)
{
}

void ServiceOrientation::Initialize(void)
{
    this->mMpu9150.Initialize();
}

void ServiceOrientation::Update(const uint32_t currentTime)
{
    this->mMpu9150.Update(currentTime);
}
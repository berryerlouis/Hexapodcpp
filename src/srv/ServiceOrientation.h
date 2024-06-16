#pragma once

#include "../cmp/Mpu9150Interface.h"
#include "Service.h"


using namespace Component;

class ServiceOrientation : public Service {
public:
	ServiceOrientation(Mpu9150Interface &mMpu9150);
	~ServiceOrientation() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

private:
	Mpu9150Interface &mMpu9150;
};

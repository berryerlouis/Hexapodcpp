#pragma once

#include "../cmp/Mpu9150.h"
#include "Service.h"

class ServiceOrientation : public Service {
public:
	ServiceOrientation(Mpu9150 &mMpu9150);
	~ServiceOrientation() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

private:
	Mpu9150 &mMpu9150;
};

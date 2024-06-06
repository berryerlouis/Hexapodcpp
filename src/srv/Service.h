#pragma once

#include "ServiceInterface.h"

class Service : public ServiceInterface {
public:
	Service(uint32_t updateTime)
		: mUpdateTime(updateTime)
		, mPreviousTime(0UL)
	{
	}

	~Service() = default;

	virtual void Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;


	bool IsTimeToUpdate (const uint32_t currentTime) const
	{
		return ( (currentTime - this->mPreviousTime) >= this->mUpdateTime);
	}

	void SetNewUpdateTime (const uint32_t currentTime)
	{
		this->mPreviousTime = currentTime;
	}

private:
	uint32_t mUpdateTime;
	uint32_t mPreviousTime;
};

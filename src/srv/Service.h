#pragma once

#include "ServiceInterface.h"

class Service : public ServiceInterface, public BaseComponent {
public:
	Service(uint32_t updateTime)
		: mUpdateTime(updateTime)
		, mPreviousTime(0UL)
	{
	}

	~Service() = default;

	Core::CoreStatus IsTimeToUpdate (const uint32_t currentTime) const
	{
		return ( (currentTime - this->mPreviousTime) >= this->mUpdateTime) ? Core::CoreStatus::CORE_OK : Core::CoreStatus::CORE_ERROR;
	}

	void SetNewUpdateTime (const uint32_t currentTime)
	{
		this->mPreviousTime = currentTime;
	}

private:
	uint32_t mUpdateTime;
	uint32_t mPreviousTime;
};

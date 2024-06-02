#pragma once

#include <stdint.h>


class ServiceInterface {
public:
	ServiceInterface(void) = default;
	~ServiceInterface()    = default;

	virtual void Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;
};

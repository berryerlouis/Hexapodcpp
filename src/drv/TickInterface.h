#pragma once
#include <stdint.h>

namespace Driver {
class TickInterface
{
public:
	TickInterface()  = default;
	~TickInterface() = default;

	virtual uint64_t GetUs(void)           = 0;
	virtual uint32_t GetMs(void)           = 0;
	virtual void DelayMs(uint16_t delayMs) = 0;
	virtual void DelayUs(uint32_t delayUs) = 0;
};
}

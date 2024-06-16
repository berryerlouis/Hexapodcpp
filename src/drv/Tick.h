#pragma once

#include "TickInterface.h"

namespace Driver {
class Tick : public TickInterface
{
public:
	Tick();
	~Tick() = default;

	virtual uint64_t GetUs(void) final override;
	virtual uint32_t GetMs(void) final override;
	virtual void DelayMs(uint16_t delayMs) final override;
	virtual void DelayUs(uint32_t delayUs) final override;

	static volatile uint64_t tick;
};
}

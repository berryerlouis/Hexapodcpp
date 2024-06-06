#pragma once
#include <stdint.h>

class Tick
{
public:
	Tick();
	~Tick() = default;
	static Tick&GetInstance(void);

	uint64_t GetUs(void);
	uint32_t GetMs(void);
	void DelayMs(uint16_t delayMs);
	void DelayUs(uint32_t delayUs);

	static volatile uint64_t tick;
	static Tick mInstance;
private:
};

extern Tick MyTick;

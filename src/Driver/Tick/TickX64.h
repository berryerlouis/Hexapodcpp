#pragma once

#include "TickInterface.h"
#ifdef GTEST
#include <chrono>
#endif

namespace Driver
{
	namespace Tick
	{
		class Tick : public TickInterface
		{
		public:
			Tick(){
                this->now = std::chrono::system_clock::now();
            }
			~Tick() = default;

			virtual uint64_t GetUs(void) final override
			{
				return 0;
			}

			virtual uint64_t GetMs(void) final override
			{
                auto duration = this->now.time_since_epoch();
				return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
			}

			virtual void DelayMs(uint64_t delayMs) final override
			{
				(void)delayMs;
			}

			virtual void DelayUs(uint64_t delayUs) final override
			{
				(void)delayUs;
			}
            std::chrono::time_point<std::chrono::system_clock> now;
		};
	}
}

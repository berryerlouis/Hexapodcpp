#pragma once

#include "GpioInterface.h"

namespace Driver
{
	namespace Gpio
	{
		class Gpio : public GpioInterface
		{
		public:
			Gpio(const SGpio& gpio, const EPortDirection& portDirection) :
				mGpio(gpio)
			{
                (void)portDirection;
			};
			~Gpio() = default;

			virtual Core::CoreStatus Set(void) final override
			{
				return (Core::CoreStatus::CORE_OK);
			}

			virtual Core::CoreStatus Reset(void) final override
			{
				return (Core::CoreStatus::CORE_OK);
			}

			virtual bool Get(void) final override
			{
				return (Core::CoreStatus::CORE_OK);
			}

			virtual EPin& GetPin(void) final override
			{
				return (this->mGpio.pin);
			}

			virtual void SetInterruptPin(void) final override
			{
			}

			virtual void ResetInterruptPin(void) final override
			{
			}

		private:
			SGpio mGpio;
		};
	}
}

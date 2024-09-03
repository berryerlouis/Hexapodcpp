#pragma once

#include "InputCaptureInterface.h"
#include "../Tick/TickInterface.h"
#include "../Gpio/GpioInterface.h"

namespace Driver
{
	namespace InputCapture
	{
		class InputCapture : public InputCaptureInterface
		{
		public:
			InputCapture(Gpio::GpioInterface& gpio, Tick::TickInterface& tick) {
                (void)gpio;
                (void)tick;
            }
			~InputCapture() = default;

			virtual Core::CoreStatus Initialize(void) final override
			{
				return (Core::CoreStatus::CORE_OK);
			}

			virtual void Update(const uint64_t currentTime) final override
			{
				(void)currentTime;
			}

			virtual void EdgeChange(void) final override
			{
			}

			virtual uint64_t GetInputCaptureTime(void) final override
			{
				return 0;;
			}
		};
	}
}

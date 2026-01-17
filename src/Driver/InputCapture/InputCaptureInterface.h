#pragma once

#include "../DriverInterface.h"

namespace Driver
{
    namespace InputCapture
    {
        class InputCaptureInterface : public DriverInterface {
        public:
            InputCaptureInterface() = default;

            ~InputCaptureInterface() = default;

            virtual void     EdgeChange(void) = 0;

            virtual uint64_t GetInputCaptureTime(void) = 0;

            virtual void     ResetInputCaptureTime(void) = 0;
        };
    } // namespace InputCapture
} // namespace Driver

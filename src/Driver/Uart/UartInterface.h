#pragma once

#include "../DriverInterface.h"
#include "Constants.h"

namespace Driver
{
    namespace Uart
    {
        class UartInterface : public DriverInterface {
        public:
            UartInterface() = default;

            ~UartInterface() = default;

            virtual Core::CoreStatus Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual void Send(const char *data, const size_t len) = 0;

            virtual void Send(const uint8_t *data, const size_t len) = 0;

            virtual void Send(const uint8_t data) = 0;

            virtual uint8_t Read(void) = 0;

            virtual uint8_t DataAvailable(void) const = 0;

        private:
        };
    }
}

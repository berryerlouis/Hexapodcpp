#pragma once

#include "UartInterface.h"
#include "../../Misc/Buffer/Buffer.h"

namespace Driver
{
    namespace Uart
    {
        class Uart : public UartInterface {
        public:
            Uart(const EBaudRate &baud = BAUDRATE_115200);

            ~Uart() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Send(const char *data, const size_t len) final override;

            virtual void Send(const uint8_t data) final override;

            virtual uint8_t Read(void) final override;

            virtual uint8_t DataAvailable(void) const final override;

            static Misc::Buffer::Buffer bufferTx;
            static Misc::Buffer::Buffer bufferRx;

        private:
            EBaudRate mBaudRate;
        };
    }
}

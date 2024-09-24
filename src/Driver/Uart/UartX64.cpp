#include "Uart.h"
#include <stdio.h>
#include <iostream>

namespace Driver
{
    namespace Uart
    {
        //Misc::Buffer::Buffer<uint8_t> Uart::bufferRx;
        //Misc::Buffer::Buffer<uint8_t> Uart::bufferTx;

        Uart::Uart(const EBaudRate &baud) : mBaudRate(baud) {
        }

        Core::CoreStatus Uart::Initialize(void) {
            setbuf(stdout, nullptr);
            return (Core::CoreStatus::CORE_OK);
        }

        void Uart::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Uart::Send(const char *data, const size_t len) {
            std::cout.write(data, len);
            std::cout << std::endl;
        }

        void Uart::Send(const uint8_t data) {
            std::cout << data << std::endl;
            (void) data;
        }

        uint8_t Uart::Read(void) {
            return 0U;
        }

        uint8_t Uart::DataAvailable(void) const {
            return 0U;
        }
    }
}

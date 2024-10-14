#include <cstring>

#include "Uart.h"
#include <stdio.h>
#include <iostream>

namespace Driver
{
    namespace Uart
    {
        uint8_t nbData = 0U;
        const char *data = "<030000>";

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
            std::cout.write(data, len) << std::endl;
        }

        void Uart::Send(const uint8_t data) {
            std::cout << data << std::endl;
        }

        uint8_t Uart::Read(void) {
            nbData++;
            if (nbData == strlen(data)) {
                nbData = 0;
            }
            return data[nbData];
        }

        uint8_t Uart::DataAvailable(void) const {
            return strlen(data);
        }
    }
}

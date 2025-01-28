#include "Uart.h"

namespace Driver
{
    namespace Uart
    {
        Uart::Uart(const EBaudRate &baud) {
            (void) baud;
        }

        Core::Status Uart::Initialize(void) {
            return (Core::Status::CORE_OK);
        }

        void Uart::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Uart::Send(const char *data, const size_t len) {
        }

        void Uart::Send(const uint8_t data) {
        }

        uint8_t Uart::Read(void) {
            uint8_t value = 0xFFU;
            return value;
        }

        uint8_t Uart::DataAvailable(void) {
            return 0U;
        }
    } // namespace Uart
} // namespace Driver

#include "Uart.h"

namespace Driver
{
    namespace Uart
    {
        Misc::Buffer::Buffer Uart::bufferRx;
        Misc::Buffer::Buffer Uart::bufferTx;

        Uart::Uart(const EBaudRate &baud) : mBaudRate(baud) {
        }

        Core::CoreStatus Uart::Initialize(void) {
            return (Core::CoreStatus::CORE_OK);
        }

        void Uart::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Uart::Send(const char *data, const size_t len) {
            (void) data;
            (void) len;
        }

        void Uart::Send(const uint8_t *data, const size_t len) {
            (void) data;
            (void) len;
        }

        void Uart::Send(const uint8_t data) {
            (void) data;
        }

        uint8_t Uart::Read(void) {
            return (Uart::bufferRx.Pop());
        }

        uint8_t Uart::DataAvailable(void) const {
            return (Uart::bufferRx.GetLength());
        }
    }
}

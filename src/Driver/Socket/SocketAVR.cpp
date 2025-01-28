#include "Socket.h"

namespace Driver
{
    namespace Socket
    {

        Socket::Socket(void):
            sin_size(0U) {
        }

        Core::Status Socket::Initialize(void) {
            return (Core::Status::CORE_OK);
        }

        void Socket::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Socket::Send(const char *data, const size_t len) {
        }

        void Socket::Send(const uint8_t data) {
        }

        uint8_t Socket::Read(void) {
            return 0xFFU;
        }

        uint8_t Socket::DataAvailable(void) {
            return 0U;
        }
    } // namespace Socket
} // namespace Driver

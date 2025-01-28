#include "SocketInterface.h"

struct addrinfo;

namespace Driver
{
    namespace Socket
    {
        class Socket : public SocketInterface {

        public:
            Socket(void);

            ~Socket() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Send(const char *data, const size_t len) final override;

            virtual void Send(const uint8_t data) final override;

            virtual uint8_t Read(void) final override;

            virtual uint8_t DataAvailable(void) final override;
        };
    } // namespace Socket
} // namespace Driver

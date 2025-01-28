#include "SocketInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

struct addrinfo;

namespace Driver
{
    namespace Socket
    {
#define BUFFER_SIZE 256U

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

        private:
            static int mServerFd;
            static int mClientFd;
            static sockaddr_storage their_addr;
            addrinfo hints, *servinfo; // connector's address information
            socklen_t sin_size;
            struct sigaction sa;
            addrinfo *p;
        };
    } // namespace Socket
} // namespace Driver

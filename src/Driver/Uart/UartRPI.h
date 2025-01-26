#include "UartInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <signal.h>

struct addrinfo;

namespace Driver
{
    namespace Uart
    {
#define BUFFER_SIZE 256U

        class Uart : public UartInterface {

        public:
            Uart(const EBaudRate &baud = BAUDRATE_115200);

            ~Uart() = default;

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
    } // namespace Uart
} // namespace Driver

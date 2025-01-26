#include "Uart.h"

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

namespace Driver
{
    namespace Uart
    {

        char bufferSocket[BUFFER_SIZE] = {0};
        uint8_t nbData = 0, nbDataRead = 0;

        int Uart::mServerFd = -1;
        int Uart::mClientFd = -1;
        sockaddr_storage Uart::their_addr;

        void sigchld_handler(int s) {
            // waitpid() might overwrite errno, so we save and restore it:
            int saved_errno = errno;
            while (waitpid(-1, nullptr, WNOHANG) > 0);
            errno = saved_errno;
        }

        Uart::Uart(const EBaudRate &baud) {
            (void) baud;

            const int yes = 1;

            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE; // use my IP

            const int rv = getaddrinfo(nullptr, "8080", &hints, &servinfo);
            if (rv != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                return;
            }
            // loop through all the results and bind to the first we can
            for (p = servinfo; p != nullptr; p = p->ai_next) {
                if ((Uart::mServerFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                    perror("server: socket");
                    continue;
                }

                if (setsockopt(Uart::mServerFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                    perror("setsockopt");
                    close(Uart::mServerFd);
                    continue;
                }

                if (bind(Uart::mServerFd, p->ai_addr, p->ai_addrlen) == -1) {
                    perror("server: bind");
                    close(Uart::mServerFd);
                    continue;
                }

                break;
            }

            freeaddrinfo(servinfo); // all done with this structure

            if (p == nullptr) {
                fprintf(stderr, "server: failed to bind\n");
                exit(1);
            }

            if (listen(Uart::mServerFd, 10) == -1) {
                perror("listen");
                exit(1);
            }

            sa.sa_handler = sigchld_handler; // reap all dead processes
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESTART;
            if (sigaction(SIGCHLD, &sa, nullptr) == -1) {
                perror("sigaction");
                exit(1);
            }

        }

        Core::Status Uart::Initialize(void) {
            return (Core::Status::CORE_OK);
        }

        void Uart::Update(const uint64_t currentTime) {
            (void) currentTime;
            if (Uart::mClientFd == -1) {
                sin_size = sizeof(Uart::their_addr);
                Uart::mClientFd = accept(Uart::mServerFd, (sockaddr *) &Uart::their_addr, &sin_size);
                if (Uart::mClientFd == -1) {
                    perror("accept");
                } else {
                    fcntl(Uart::mClientFd, F_SETFL, O_NONBLOCK);
                }
            }
        }

        void Uart::Send(const char *data, const size_t len) {
            if (Uart::mClientFd > 0) {
                ssize_t bytesSent = write(Uart::mClientFd, data, len);
                if (bytesSent == -1) {
                    perror("write");
                } else {
                    std::cout.write(data, len) << std::endl;
                }
            }
        }

        void Uart::Send(const uint8_t data) {
            if (Uart::mClientFd > 0) {
                const ssize_t bytesSent = write(Uart::mClientFd, &data, 1U);
                if (bytesSent == -1) {
                    perror("write");
                } else {
                    std::cout << data << std::endl;
                }
            }
        }

        uint8_t Uart::Read(void) {
            uint8_t value = 0xFFU;
            if (Uart::mClientFd > 0 && nbData > 0) {
                nbData--;
                value = bufferSocket[nbDataRead];
                nbDataRead++;
                if (nbData == 0) {
                    nbDataRead = 0;
                }
            }
            return value;
        }

        uint8_t Uart::DataAvailable(void) {
            if (Uart::mClientFd > 0) {
                int count;
                const ssize_t bytesRead = read(Uart::mClientFd, bufferSocket, BUFFER_SIZE);
                if (bytesRead > 0) {
                    nbData += bytesRead;
                } else if (bytesRead == 0) {
                    Uart::mClientFd = -1;
                } else if (bytesRead == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("read");
                }
            }
            return nbData;
        }
    } // namespace Uart
} // namespace Driver

#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include "UartX64.h"


namespace Driver
{
    namespace Uart
    {
        Misc::Buffer::Buffer mBufferRx;

        std::string          GetLineFromCin() {
            std::string line;
            std::getline(std::cin, line);
            return line;
        }

        char                     buffer[255U];
        std::future<std::string> future;

        Uart::Uart(const EBaudRate &baud) {
            (void) baud;
            future = std::async(std::launch::async, GetLineFromCin);
        }

        Core::Status Uart::Initialize(void) {
            return (Core::Status::CORE_OK);
        }

        void Uart::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Uart::Send(const char  *data,
                        const size_t len) {
            std::cout << data << std::endl;
        };

        void Uart::Send(const uint8_t data) {
            std::cout << data << std::endl;
        };

        uint8_t Uart::Read(void) {
            return mBufferRx.Pop();
        }

        uint8_t Uart::DataAvailable(void) {
            if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                const std::string line = future.get();
                uint8_t           buffer[line.length()];

                // Set a new line. Subtle race condition between the previous line
                // and this. Some lines could be missed. To aleviate, you need an
                // io-only thread. I'll give an example of that as well.
                future = std::async(std::launch::async, GetLineFromCin);

                sprintf(reinterpret_cast<char *>(buffer), "%s", line.c_str());
                for (int i = 0; i < line.length(); ++i) {
                    mBufferRx.Push(buffer[i]);
                }
            }
            return mBufferRx.GetLength();
        };
    } // namespace Uart
} // namespace Driver

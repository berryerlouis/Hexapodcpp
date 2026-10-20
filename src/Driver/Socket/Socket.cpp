#include "Socket.h"
#include <cstring>
#include <stdint.h>
#include "tiny_websockets/server.hpp"

namespace Driver
{
    namespace Socket
    {
        websockets::WebsocketsServer         server;
        websockets::WebsocketsClient         client;
        static websockets::WSInterfaceString bufferMessage;
        static bool                          once = false;
        static constexpr size_t              RX_FRAME_SIZE = 50U;
        static constexpr size_t              RX_QUEUE_CAPACITY = 32U;
        static char                          sBufferRx[RX_FRAME_SIZE] = {0U};
        static char                          sReceivedFrames[RX_QUEUE_CAPACITY][RX_FRAME_SIZE] = {{0U}};
        static uint8_t                       sIndexBufferRx = 0U;
        static bool                          sBeginIncomingFrame = false;
        static size_t                        sRxReadIndex = 0U;
        static size_t                        sRxWriteIndex = 0U;
        static size_t                        sRxCount = 0U;

        void Socket::onMessage(const websockets::WebsocketsClient &client,
                               websockets::WebsocketsMessage       message) {
            (void) client;
            bufferMessage += message.data();
            while (ReceivedStringFrame()) {
            }
        }

        void Socket::onEvent(const websockets::WebsocketsClient &client,
                             websockets::WebsocketsEvent         event,
                             websockets::WSInterfaceString       data) {
            (void) client;
            (void) data;
            if (event == websockets::WebsocketsEvent::ConnectionOpened ||
                event == websockets::WebsocketsEvent::ConnectionClosed) {
                bufferMessage.clear();
                once = false;
                sIndexBufferRx = 0U;
                sBeginIncomingFrame = false;
                sRxReadIndex = 0U;
                sRxWriteIndex = 0U;
                sRxCount = 0U;
            }
        }

        Core::Status Socket::Initialize() {
            server.listen(8080);
            LOG_DRIVER_DEBUG("Socket", "Initialized.");
            return (Core::Status::CORE_OK);
        }

        void Socket::Update(const uint64_t currentTime) {
            (void) currentTime;
            if (server.available()) {
                if (server.poll()) {
                    client = server.accept();
                    client.onMessage(onMessage);
                    client.onEvent(onEvent);
                    this->Notify(CLIENT_CONNECTED);
                }
                if (!client.available()) {
                    if (!once) {
                        this->Notify(NO_CLIENT);
                        once = true;
                    }
                }
                client.poll();
            }
        }

        void Socket::Send(const char *data, const size_t len) {
            if (client.available()) {
                client.send(data, len);
            }
        }

        void Socket::Send(const uint8_t data) {
            if (client.available()) {
                client.send(reinterpret_cast<const char *>(&data), 1U);
            }
        }

        bool Socket::HasNewFrame(void) {
            if (sRxCount == 0U) {
                return false;
            }

            sRxReadIndex = (sRxReadIndex + 1U) % RX_QUEUE_CAPACITY;
            sRxCount -= 1U;
            return true;
        }

        volatile char *Socket::ReadIncomingData(void) {
            const size_t currentFrameIndex =
                    (sRxReadIndex + RX_QUEUE_CAPACITY - 1U) % RX_QUEUE_CAPACITY;
            return reinterpret_cast<volatile char *>(sReceivedFrames[currentFrameIndex]);
        }

        uint8_t Socket::Read() {
            if (false == bufferMessage.empty()) {
                const uint8_t value = bufferMessage.c_str()[0U];
                bufferMessage = bufferMessage.substr(1);
                return value;
            }
            return 0xFFU;
        }

        bool Socket::ReceivedStringFrame() {
            uint8_t nbData = bufferMessage.length();
            while (nbData != 0U) {
                nbData--;
                const volatile uint8_t rc = Read();
                if (rc == '<') {
                    sBeginIncomingFrame = true;
                    sIndexBufferRx = 0U;
                } else if (rc != '>') {
                    if ((rc >= '0' && rc <= '9') || (rc >= 'A' && rc <= 'F')) {
                        if (sIndexBufferRx < sizeof(sBufferRx) - 1) {
                            sBufferRx[sIndexBufferRx++] = rc;
                        } else {
                            sIndexBufferRx = 0U;
                            sBeginIncomingFrame = false;
                        }
                    } else {
                        sIndexBufferRx = 0U;
                        sBeginIncomingFrame = false;
                    }
                } else {
                    if (sBeginIncomingFrame && sIndexBufferRx >= 6U &&
                        (sIndexBufferRx & 0x01U) == 0U) {
                        sBufferRx[sIndexBufferRx] = '\0';
                        sIndexBufferRx = 0U;
                        sBeginIncomingFrame = false;
                        return PushReceivedFrame();
                    }
                    sIndexBufferRx = 0U;
                    sBeginIncomingFrame = false;
                }
            }
            return (false);
        }

        bool Socket::PushReceivedFrame() {
            if (sRxCount >= RX_QUEUE_CAPACITY) {
                LOG_DRIVER_WARNING("Socket", "RX queue full, drop frame.");
                return true;
            }

            std::memcpy(sReceivedFrames[sRxWriteIndex], sBufferRx, sizeof(sBufferRx));
            sRxWriteIndex = (sRxWriteIndex + 1U) % RX_QUEUE_CAPACITY;
            sRxCount += 1U;
            return true;
        }
    } // namespace Socket
} // namespace Driver

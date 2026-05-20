#include "Socket.h"
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
        static volatile char                 sBufferRx[50U] = {0U};
        static uint8_t                       sIndexBufferRx = 0U;
        static bool                          sBeginIncomingFrame = false;
        static bool                          sHasNewFrame = false;

        void Socket::onMessage(const websockets::WebsocketsClient &client,
                               websockets::WebsocketsMessage       message) {
            (void) client;
            bufferMessage += message.data();
            sHasNewFrame = ReceivedStringFrame();
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
            bool hasNewFrame = sHasNewFrame;
            sHasNewFrame = false;
            return hasNewFrame;
        }

        volatile char *Socket::ReadIncomingData(void) {
            return reinterpret_cast<volatile char *>(sBufferRx);
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
                        return (true);
                    }
                    sIndexBufferRx = 0U;
                    sBeginIncomingFrame = false;
                }
            }
            return (false);
        }
    } // namespace Socket
} // namespace Driver

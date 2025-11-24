#include "Socket.h"
#include "tiny_websockets/server.hpp"

namespace Driver
{
    namespace Socket
    {
        websockets::WebsocketsServer server;
        websockets::WebsocketsClient client;
        static websockets::WSInterfaceString bufferMessage;
        static bool once = false;

        void Socket::onMessage(websockets::WebsocketsClient &client, websockets::WebsocketsMessage message) {
            (void) client;
            bufferMessage += message.data();
        }

        void Socket::onEvent(websockets::WebsocketsClient &client, websockets::WebsocketsEvent event,
                             websockets::WSInterfaceString data) {
            (void) client;
            (void) data;
            if (event == websockets::WebsocketsEvent::ConnectionOpened || event ==
                websockets::WebsocketsEvent::ConnectionClosed) {
                bufferMessage.clear();
                once = false;
            }

        }

        Socket::Socket(void) {
        }

        Core::Status Socket::Initialize(void) {
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
                if (client.available() == false) {
                    if (once == false) {
                        this->Notify(NO_CLIENT);
                        once = true;
                    }
                }
                client.poll();
            }
        }

        void Socket::Send(const char *data, const size_t len) {
            if (client.available() == true) {
                client.send(data, len);
            }
        }

        void Socket::Send(const uint8_t data) {
            if (client.available() == true) {
                client.send(reinterpret_cast<const char *>(&data), 1U);
            }
        }

        uint8_t Socket::Read(void) {
            if (bufferMessage.length() > 0U) {
                const uint8_t value = bufferMessage.c_str()[0U];
                bufferMessage = bufferMessage.substr(1);
                return value;
            }
            return 0xFFU;
        }

        uint8_t Socket::DataAvailable(void) {
            return bufferMessage.length();
        }

    } // namespace Socket
} // namespace Driver

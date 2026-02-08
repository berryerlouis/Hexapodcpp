#include "Socket.h"
#include "tiny_websockets/server.hpp"

namespace Driver
{
    namespace Socket
    {
        websockets::WebsocketsServer         server;
        websockets::WebsocketsClient         client;
        static websockets::WSInterfaceString bufferMessage;
        static bool                          once = false;

        void Socket::onMessage(const websockets::WebsocketsClient &client,
                               websockets::WebsocketsMessage       message) {
            (void) client;
            bufferMessage += message.data();
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

        uint8_t Socket::Read() {
            if (!bufferMessage.empty()) {
                const uint8_t value = bufferMessage.c_str()[0U];
                bufferMessage = bufferMessage.substr(1);
                return value;
            }
            return 0xFFU;
        }

        uint8_t Socket::DataAvailable() {
            return bufferMessage.length();
        }
    } // namespace Socket
} // namespace Driver

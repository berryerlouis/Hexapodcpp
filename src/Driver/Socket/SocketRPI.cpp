#include "Socket.h"
#ifndef GTEST
#include "tiny_websockets/server.hpp"
#endif
namespace Driver
{
    namespace Socket
    {
        websockets::WebsocketsServer server;
        websockets::WebsocketsClient client;
        static websockets::WSInterfaceString bufferMessage;

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
            }
        }

        Socket::Socket(void) {
        }

        Core::Status Socket::Initialize(void) {
            server.listen(8080);
            return (Core::Status::CORE_OK);
        }

        void Socket::Update(const uint64_t currentTime) {
            (void) currentTime;
            if (server.available()) {
                if (server.poll()) {
                    client = server.accept();
                    client.onMessage(onMessage);
                    client.onEvent(onEvent);
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
                client.send((const char *) &data, 1U);
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

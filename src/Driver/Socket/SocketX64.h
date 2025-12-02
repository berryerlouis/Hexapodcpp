#include "SocketInterface.h"
#ifndef GTEST
#include "tiny_websockets/client.hpp"
#endif


struct addrinfo;

namespace Driver
{
    namespace Socket
    {
        class Socket : public SocketInterface<1U, SocketStruct> {
        public:
            Socket(void);

            ~Socket() = default;

#ifndef GTEST
            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Send(const char *data, const size_t len) final override;

            virtual void Send(const uint8_t data) final override;

            virtual uint8_t Read(void) final override;

            virtual uint8_t DataAvailable(void) final override;

            static void onMessage(websockets::WebsocketsClient &client, websockets::WebsocketsMessage message);

            static void onEvent(websockets::WebsocketsClient &client, websockets::WebsocketsEvent event,
                                websockets::WSInterfaceString data);
#else
            virtual Core::Status
            Initialize(void) final override {
                return Core::Status::CORE_OK;
            }

            virtual void
            Update(const uint64_t currentTime) final override {
            }

            virtual void
            Send(const char *data, const size_t len) final override {
            }

            virtual void
            Send(const uint8_t data) final override {
            }

            virtual uint8_t
            Read(void) final override {
                return 0;
            }

            virtual uint8_t
            DataAvailable(void) final override {
                return 0;
            }
#endif

        private:
        };
    } // namespace Socket
} // namespace Driver
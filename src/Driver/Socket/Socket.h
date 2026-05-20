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
            Socket() = default;

            ~Socket() = default;

#ifndef GTEST
            virtual Core::Status   Initialize(void) final override;

            virtual void           Update(const uint64_t currentTime) final override;

            virtual void           Send(const char *data, const size_t len) final override;

            virtual void           Send(const uint8_t data) final override;

            virtual volatile char *ReadIncomingData(void) final override;

            virtual bool           HasNewFrame(void) final override;

#else
            virtual Core::Status Initialize(void) final override {
                return Core::Status::CORE_OK;
            }

            virtual void Update(const uint64_t currentTime) final override {
            }

            virtual void Send(const char *data, const size_t len) final override {
            }

            virtual void Send(const uint8_t data) final override {
            }

            virtual volatile char *ReadIncomingData(void) final override {
                return nullptr;
            }

            virtual bool HasNewFrame(void) final override {
                return false;
            }
#endif

        private:
            static uint8_t Read(void);

            static bool    ReceivedStringFrame(void);

            static void    onMessage(const websockets::WebsocketsClient &client,
                                     websockets::WebsocketsMessage       message);

            static void    onEvent(const websockets::WebsocketsClient &client,
                                   websockets::WebsocketsEvent         event,
                                   websockets::WSInterfaceString       data);
        };
    } // namespace Socket
} // namespace Driver

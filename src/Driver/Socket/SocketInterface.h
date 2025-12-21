#pragma once

#include "../../Core/Observable.h"
#include "../DriverInterface.h"

namespace Driver
{
    namespace Socket
    {
        enum SocketStruct {
            NO_CLIENT = 0U,
            CLIENT_CONNECTED = 1U,
        };


        template<uint8_t MAX_OBSERVERS, class T>
        class SocketInterface : public DriverInterface, public Core::Observable<MAX_OBSERVERS, T> {
        public:
            SocketInterface() = default;

            ~SocketInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void         Update(const uint64_t currentTime) = 0;

            virtual void         Send(const char  *data,
                                      const size_t len) = 0;

            virtual void         Send(const uint8_t data) = 0;

            virtual uint8_t      Read(void) = 0;

            virtual uint8_t      DataAvailable(void) = 0;

        private:
        };
    } // namespace Socket
} // namespace Driver

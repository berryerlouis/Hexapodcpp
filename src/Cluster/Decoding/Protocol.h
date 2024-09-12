#pragma once

#include "../Frame/Frame.h"

namespace Cluster
{
    namespace Decoding
    {
        class Protocol {
        public:
            enum ProtocolStatus {
                NO_ERROR,
                ERROR_LENGHT,
                ERROR_CHAR_INVALID,
                ERROR_SIZE_PARAMS,
                ERROR_NULL_BUFFER,
            };

            Protocol(void);

            ~Protocol() = default;

            static ProtocolStatus Decode(const char *frameBuffer, Frame &frame);

            static uint8_t Encode(const Frame &response, const char *buffer);

        private:
            static uint8_t ConvertHexCharToInt(uint8_t byte);
        };
    }
}

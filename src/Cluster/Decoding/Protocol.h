#pragma once

#include "../Frame/Frame.h"

namespace Cluster
{
    namespace Decoding
    {
        class Protocol {
        public:
            Protocol(void);

            ~Protocol() = default;

            static Core::CoreStatus Decode(const char *frameBuffer, Frame &frame);

            static uint8_t Encode(const Frame &response, char *buffer);

        private:
            static uint8_t ConvertHexCharToInt(const uint8_t byte);
        };
    }
}

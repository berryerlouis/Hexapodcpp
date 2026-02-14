#pragma once

#include "../Frame/Frame.h"

namespace Component
{
    namespace Decoding
    {
        class Protocol {
        public:
            Protocol(void) = default;

            ~Protocol() = default;

            static Core::Status Decode(const char *frameBuffer, Frame &frame);

            static uint8_t      Encode(const Frame &response, char *buffer);
        };
    } // namespace Decoding
} // namespace Component

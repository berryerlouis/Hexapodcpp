#pragma once

#include "../ComponentInterface.h"

namespace Component
{
    namespace Software
    {
        class SoftwareInterface : public ComponentInterface {
        public:
            struct Version {
                uint8_t major;
                uint8_t minor;
            };

            SoftwareInterface() = default;

            ~SoftwareInterface() = default;

            virtual Core::CoreStatus Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual Version GetVersion(void) = 0;
        };
    }
}

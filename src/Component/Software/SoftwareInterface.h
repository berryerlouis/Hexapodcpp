#pragma once

#include "../ComponentInterface.h"

namespace Component
{
    namespace Software
    {
        class SoftwareInterface : public ComponentInterface<0U, std::nullptr_t> {
        public:
            struct Version {
                uint8_t major;
                uint8_t minor;
            };

            SoftwareInterface() = default;

            ~SoftwareInterface() = default;

            virtual Version  GetVersion(void) = 0;

            virtual uint64_t GetMinTime(void) const = 0;

            virtual uint64_t GetMaxTime(void) const = 0;

            virtual void     ResetTime(void) = 0;
        };
    } // namespace Software
} // namespace Component

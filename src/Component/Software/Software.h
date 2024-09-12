#pragma once

#include "SoftwareInterface.h"

namespace Component
{
    namespace Software
    {
        class Software : public SoftwareInterface {
        public:
            Software();

            ~Software() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual Version GetVersion(void) final override;
        };
    }
}

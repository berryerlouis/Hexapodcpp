#pragma once

#include "../ComponentInterface.h"
#include "SoundState.h"

namespace Component
{
    namespace Sound
    {
        class SoundInterface : public ComponentInterface<5U, SoundStruct> {
        public:
            SoundInterface() = default;

            ~SoundInterface() = default;

            virtual uint64_t GetIntervalSoundHit(void) const = 0;
        };
    } // namespace Sound
} // namespace Component

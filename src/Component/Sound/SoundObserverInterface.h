#pragma once

#include "SoundState.h"
#include "SoundsId.h"

namespace Component
{
    namespace Sound
    {
        class SoundObserverInterface {
        public:
            SoundObserverInterface() = default;

            ~SoundObserverInterface() = default;

            virtual void UpdatedSoundState(const SoundId &soundId, const SoundState &soundState,
                                           const uint16_t period) = 0;
        };
    }
}

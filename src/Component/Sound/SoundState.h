#pragma once
#include <cstdint>

namespace Component
{
    namespace Sound
    {
        static constexpr uint8_t NB_SENSORS_SOUND = 2U;

        enum SoundId {
            SOUND_LEFT = 0x00U,
            SOUND_RIGHT,
            SOUND_NONE
        };

        enum SoundState {
            NO_SOUND = 0U,
            LOUD
        };

        struct SoundStruct {
            const SoundId &id;
            const SoundState state;
            const uint64_t delay;
        };
    }
}

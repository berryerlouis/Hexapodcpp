#pragma once

#include "SoundObserverInterface.h"

namespace Component
{
    namespace Sound
    {
        class SoundObservableInterface {
        public:
            SoundObservableInterface() = default;

            ~SoundObservableInterface() = default;

            virtual Core::Status Attach(SoundObserverInterface *observer) = 0;

            virtual void Notify(const SoundId &soundId, const SoundState &soundState, const uint16_t period) = 0;
        };
    }
}

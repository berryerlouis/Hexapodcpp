#pragma once

#include "SoundObservable.h"
#include "../ComponentInterface.h"

namespace Component
{
    namespace Sound
    {

        class SoundInterface : public ComponentInterface, public SoundObservableInterface {
        public:
            SoundInterface() = default;

            ~SoundInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual SoundState Get() const = 0;

            virtual Core::Status Attach(SoundObserverInterface *observer) = 0;

            virtual void Notify(const SoundId &soundId, const SoundState &soundState, const uint16_t period) = 0;
        };
    }
}

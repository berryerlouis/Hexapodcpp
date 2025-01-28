#pragma once

#include "../Service.h"
#include "../../Component/Sound/SoundInterface.h"
#include "../../Component/Sound/SoundObserverInterface.h"

namespace Service
{
    namespace Sound
    {
        using namespace Component::Sound;

        class ServiceSound : public Service, public SoundObserverInterface {
        public:
            ServiceSound(SoundInterface &soundInterfaceLeft, SoundInterface &soundInterfaceRight,
                         Event::MessageInterface &messageListener);

            ~ServiceSound() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void UpdatedSoundState(const SoundId &soundId, const SoundState &SoundState,
                                           const uint16_t period) final override;

        protected:
            SoundInterface &mSoundInterfaceLeft;
            SoundInterface &mSoundInterfaceRight;
        };
    }
}

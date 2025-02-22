#pragma once

#include "../Service.h"
#include "../../Component/Sound/Sound.h"

namespace Service
{
    namespace Sound
    {
        using namespace Component;
        using namespace Component::Sound;

        class ServiceSound : public Service {
        public:
            ServiceSound(SoundInterface &soundInterfaceLeft, SoundInterface &soundInterfaceRight,
                         Event::MessageInterface &messageListener);

            ~ServiceSound() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

        private:
            void SendMaxSound(void) const;

            SoundInterface &mSoundLeft;
            SoundInterface &mSoundRight;
        };
    }
}

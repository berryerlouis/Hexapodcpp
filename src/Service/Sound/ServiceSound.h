#pragma once

#include "../Service.h"
#include "../../Component/Sound/SoundInterface.h"
#include "../../Component/ObserverInterface.h"

namespace Service
{
    namespace Sound
    {
        using namespace Component;
        using namespace Component::Sound;

        class ServiceSound : public Service, ObserverInterface<SoundStruct> {
        public:
            ServiceSound(SoundInterface &soundInterfaceLeft, SoundInterface &soundInterfaceRight,
                         Event::MessageInterface &messageListener);

            ~ServiceSound() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Notified(const SoundStruct &sound) final override;

        protected:
            SoundInterface &mSoundInterfaceLeft;
            SoundInterface &mSoundInterfaceRight;
        };
    }
}

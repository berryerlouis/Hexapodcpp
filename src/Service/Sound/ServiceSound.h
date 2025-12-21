#pragma once

#include "../../Component/Sound/Sound.h"
#include "../Service.h"

namespace Service
{
    namespace Sound
    {
        using namespace Component;
        using namespace Component::Sound;

        class ServiceSound : public Service {
        public:
            ServiceSound(SoundInterface                &soundInterfaceLeft,
                         SoundInterface                &soundInterfaceRight,
                         Message::MessageInterface     &messageListener,
                         Event::EventListenerInterface &eventListener);

            ~ServiceSound() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         DispatchEvent(const Event::Event &event) final override;

        private:
            void            SendMaxSound(void) const;

            SoundInterface &mSoundLeft;
            SoundInterface &mSoundRight;
        };
    } // namespace Sound
} // namespace Service

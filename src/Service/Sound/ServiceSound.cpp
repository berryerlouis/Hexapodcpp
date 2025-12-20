#include "ServiceSound.h"

#include "../../Cluster/Sound/ClusterSound.h"

namespace Service
{
    namespace Sound
    {
        ServiceSound::ServiceSound(
                SoundInterface &soundInterfaceLeft,
                SoundInterface &soundInterfaceRight,
                Message::MessageInterface &messageListener,
                Event::EventListenerInterface &eventListener
        ) :
            Service(SOUND, 500U, messageListener, eventListener),
            mSoundLeft(soundInterfaceLeft),
            mSoundRight(soundInterfaceRight) {
        }

        Core::Status ServiceSound::Initialize(void) {
            const Core::Status successLeft = this->mSoundLeft.Initialize();
            const Core::Status successRight = this->mSoundRight.Initialize();
            Core::Status success = Core::Status::CORE_ERROR;
            if (Core::IsSuccess(successLeft) && Core::IsSuccess(successRight)) {
                this->mInitialized = true;
                success = Core::CORE_OK;
            }
            return (success);
        }

        void ServiceSound::Update(const uint64_t currentTime) {
            this->mSoundLeft.Update(currentTime);
            this->mSoundRight.Update(currentTime);
            this->SendMaxSound();
        }

        void ServiceSound::SendMaxSound(void) const {
            const SoundStruct maxSound = Component::Sound::Sound::ComputeAndNotifyMaxSound();
            if (maxSound.id != SOUND_NONE) {
                Frame response;
                Cluster::Sound::ClusterSound::BuildFrameGetSoundState(maxSound.id, maxSound.delay, response);
                this->SendMessage(response);
            }
        }

        void ServiceSound::DispatchEvent(const Event::EventType event) const {
            (void) event;
        }
    } // namespace Sound
} // namespace Service

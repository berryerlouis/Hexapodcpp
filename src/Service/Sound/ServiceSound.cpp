#include "ServiceSound.h"

#include "../../Cluster/Sound/ClusterSound.h"

namespace Service
{
    namespace Sound
    {
        ServiceSound::ServiceSound(SoundInterface &soundInterfaceLeft, SoundInterface &soundInterfaceRight,
                                   Event::MessageInterface &messageListener) :
            Service(10U, messageListener)
            , mSoundInterfaceLeft(soundInterfaceLeft)
            , mSoundInterfaceRight(soundInterfaceRight) {
        }

        Core::Status ServiceSound::Initialize(void) {
            const Core::Status successLeft = this->mSoundInterfaceLeft.Initialize();
            const Core::Status successRight = this->mSoundInterfaceRight.Initialize();
            const Core::Status success = (Core::Status::CORE_OK == successLeft) && (
                                             Core::Status::CORE_OK == successRight)
                                             ? Core::Status::CORE_OK
                                             : Core::Status::CORE_ERROR;
            if (Core::Status::CORE_OK == success) {
                this->mSoundInterfaceLeft.Attach(this);
                this->mSoundInterfaceRight.Attach(this);
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceSound::Update(const uint64_t currentTime) {
            this->mSoundInterfaceLeft.Update(currentTime);
            this->mSoundInterfaceRight.Update(currentTime);
        }

        void ServiceSound::UpdatedSoundState(const SoundId &soundId, const SoundState &SoundState,
                                             const uint16_t period) {
            Frame response;
            Cluster::Sound::ClusterSound::BuildFrameGetSoundState(soundId, SoundState, response);
            this->SendMessage(response);
        }
    } // namespace Sound
} // namespace Service

#include "ClusterSound.h"


namespace Cluster
{
    namespace Sound
    {
        using namespace Component::Sound;

        ClusterSound::ClusterSound(SoundInterface &soundLeft,
                                   SoundInterface &soundRight) :
            ClusterBase(SOUND,
                        *this),
            ClusterCommand(NB_COMMANDS_SOUND),
            mSoundLeft(soundLeft),
            mSoundRight(soundRight) {
            this->AddClusterItem({.commandId = ESoundCommands::GET_SOUND_STATUS, .expectedSize = 1U});
            LOG_CLUSTER_DEBUG("Sound", "(%d) Initialized.", SOUND);
        }

        Core::Status ClusterSound::ExecuteFrame(const Frame &request,
                                                Frame       &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.GetCommandId() == ESoundCommands::GET_SOUND_STATUS) {
                uint64_t      sound;
                const SoundId soundId = static_cast<SoundId>(request.Get1ByteParam(0U));
                if (soundId == SOUND_LEFT) {
                    sound = this->mSoundLeft.GetIntervalSoundHit();
                } else {
                    sound = this->mSoundRight.GetIntervalSoundHit();
                }
                success = BuildFrameGetSoundState(soundId, sound, response);
            }
            return success;
        }

        Core::Status ClusterSound::BuildFrameGetSoundState(const SoundId  &soundId,
                                                           const uint64_t &soundDelay,
                                                           Frame          &response) {
            const Core::Status success = response.Build(EClusters::SOUND, ESoundCommands::GET_SOUND_STATUS);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(soundId);
                response.Set8BytesParam(soundDelay);
            }
            return (success);
        }
    }; // namespace Sound
} // namespace Cluster

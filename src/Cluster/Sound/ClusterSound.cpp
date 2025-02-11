#include "ClusterSound.h"


namespace Cluster
{
    namespace Sound
    {
        using namespace Component::Sound;

        ClusterSound::ClusterSound(SoundInterface &soundLeft, SoundInterface &soundRight) :
            ClusterBase(SOUND, this)
            , ClusterCommand(NB_COMMANDS_SOUND)
            , mSoundLeft(soundLeft)
            , mSoundRight(soundRight) {
            this->AddClusterItem({.commandId = ESoundCommands::GET_SOUND_STATUS, .expectedSize = 1U});
        }

        Core::Status ClusterSound::ExecuteFrame(const Frame &request, Frame &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.commandId == ESoundCommands::GET_SOUND_STATUS) {
                SoundState state;
                const SoundId soundId = static_cast<SoundId>(request.params[0U]);
                if (soundId == SOUND_LEFT) {
                    state = this->mSoundLeft.GetStatus();
                } else {

                    state = this->mSoundRight.GetStatus();
                }
                success = BuildFrameGetSoundState(soundId, state, response);
            }
            return success;
        }

        Core::Status ClusterSound::BuildFrameGetSoundState(const SoundId &soundId, const SoundState &soundState,
                                                           Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SOUND,
                    ESoundCommands::GET_SOUND_STATUS);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(soundId);
                response.Set1ByteParam(soundState);
            }
            return (success);
        }

    };
}

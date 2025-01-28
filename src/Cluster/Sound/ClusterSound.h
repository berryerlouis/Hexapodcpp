#pragma once

#include "../ClusterBase.h"
#include "../../Component/Sound/SoundInterface.h"

namespace Cluster
{
    namespace Sound
    {
        using namespace Component::Sound;

        class ClusterSound : public ClusterBase, ClusterCommand {
        public:
            ClusterSound(SoundInterface &soundLeft, SoundInterface &soundRight);

            ~ClusterSound() = default;

            virtual Core::Status ExecuteFrame(const Frame &request, Frame &response) override;

            static Core::Status BuildFrameGetSoundState(const SoundId &soundId, const SoundState &soundState, Frame &response);

        private:
            SoundInterface &mSoundLeft;
            SoundInterface &mSoundRight;
        };
    }
}

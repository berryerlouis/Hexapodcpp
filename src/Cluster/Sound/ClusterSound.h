#pragma once

#include "../../Component/Sound/Sound.h"
#include "../ClusterBase.h"

namespace Cluster
{
    namespace Sound
    {
        using namespace Component::Sound;

        class ClusterSound : public ClusterBase, ClusterCommand {
        public:
            ClusterSound(SoundInterface &soundLeft,
                         SoundInterface &soundRight);

            ~ClusterSound() = default;

            virtual Core::Status ExecuteFrame(const Frame &request,
                                              Frame       &response) override;

            static Core::Status  BuildFrameGetSoundState(const SoundId  &soundId,
                                                         const uint64_t &soundDelay,
                                                         Frame          &response);

        private:
            SoundInterface &mSoundLeft;
            SoundInterface &mSoundRight;
        };
    } // namespace Sound
} // namespace Cluster

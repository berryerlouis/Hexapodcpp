#pragma once
#include "../Constants.h"
#include "../Legs/Legs.h"
#include "../../Misc/Maths/Geometry.h"

namespace Bot
{
    namespace Walk
    {
        using namespace Misc::Maths;

        class Walk {
        public:
            Walk(Legs::Legs &legs);

            ~Walk() = default;

            void UpdateStatus(const EWalkStatus status);

            void Update(const uint64_t currentTime);

        private:
            void Play(const uint64_t currentTime);

            void Pause(const uint64_t currentTime);

            void Stop(const uint64_t currentTime);

            struct Step {
                Position3d positions[4U];
                uint16_t delayStep;
                uint64_t startTime;
                uint8_t idxPosition;
            };

            Legs::Legs &mLegs;
            EWalkStatus mStatus;
            uint8_t mCounterStepPosition;
            uint8_t mMaxCounterStepPosition;
            Step mWalkStepPosition[NB_LEGS];
            uint16_t mDelayStep;
            uint64_t mPreviousTime;
            bool mStepFinished;
        };
    }
}

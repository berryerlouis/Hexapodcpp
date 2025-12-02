#pragma once

#include "Constants.h"
#include "../../Bot/Constants.h"
#include "../../Bot/Legs/LegsInterface.h"
#include "GaitParams.h"
#include <vector>

namespace Move
{
    namespace Gait
    {
        class GaitCycle {
        public:
            GaitCycle(Bot::Legs::LegsInterface &legs, GaitParams &gaitParams);

            ~GaitCycle() = default;

            // Playback control
            bool Start();

            bool Pause() const;

            bool Stop() const;

            // Update cycle
            void Update(uint64_t currentTime);

        private:
            float GetDeltaTimeOfCycleDuration(const uint64_t currentTime) const;

            float GetNormalizedTime(const uint64_t currentTime) const;

            bool IsCycleComplete(const uint64_t currentTime) const;

            void AdvanceToNextCycle(const uint64_t currentTime);

            Bot::Legs::LegsInterface &mLegs;
            GaitParams &mGaitParams;
            uint64_t mLastUpdateTime;

            uint64_t mStartTime;
            uint8_t mStepPositionIndex;
            std::vector<Misc::Maths::Position3d> mPositionsLift = {
                {0.0F, -1.0F, 0.0F},
                {0.0F, 0.0F, 1.0F},
                {0.0F, 1.0F, 0.0F}
            };
            std::vector<Misc::Maths::Position3d> mPositionsDown = {
                {0.0F, 1.0F, 0.0F},
                {0.0F, 0.0F, -0.1F},
                {0.0F, -1.0F, 0.0F}
            };
            std::vector<std::vector<Misc::Maths::Position3d> > mPositions = {mPositionsLift, mPositionsDown};
        };
    }
}
#pragma once

#include <memory>
#include <vector>
#include "../../Bot/Constants.h"
#include "../../Bot/Legs/LegsInterface.h"
#include "Constants.h"
#include "GaitParams.h"
#include "GaitStrategy.h"

namespace Move
{
    namespace Gait
    {
        class GaitCycle {
        public:
            GaitCycle(Bot::Legs::LegsInterface &legs, GaitParams &gaitParams);

            ~GaitCycle() = default;

            bool Start(void);

            bool Pause(void) const;

            bool Stop(void);

            bool SetGaitType(GaitType gaitType);

            void Update(uint64_t currentTime);

        private:
            float GetDeltaTimeOfCycleDuration(const uint64_t currentTime) const;

            float GetNormalizedTime(const uint64_t currentTime) const;

            bool  IsCycleComplete(const uint64_t currentTime) const;

            void  AdvanceToNextCycle(const uint64_t currentTime);

            Bot::Legs::LegsInterface                 &mLegs;
            GaitParams                               &mGaitParams;
            std::unique_ptr<Move::Gait::GaitStrategy> mGaitStrategy;

            uint64_t                                  mStartTime;
            uint8_t                                   mStepPositionIndex;
            std::vector<Misc::Maths::Position3d>      mPositionsLift = {
                    {0.0F, 1.0F, 0.0F},
                    {0.0F, 0.0F, -1.0F},
                    {0.0F, -1.0F, 0.0F}};
            std::vector<Misc::Maths::Position3d> mPositionsDown = {
                    {0.0F, -1.0F, 0.0F},
                    {0.0F, 0.0F, 0.1F},
                    {0.0F, 1.0F, 0.0F}};
            Misc::Maths::Position3d mPositionsStop = {0.0F, 0.0F, 0.0F};

            std::vector<std::vector<Misc::Maths::Position3d>> mPositions = {
                    mPositionsLift,
                    mPositionsDown};
        };
    } // namespace
      // Gait
} // namespace Move

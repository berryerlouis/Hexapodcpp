#pragma once

#include <cstdint>
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
            // Time to blend from the neutral pose to the full gait (and back).
            static constexpr uint64_t RAMP_DURATION_MS = 500U;
            // Longest time step integrated at once, protects against a stalled main loop.
            static constexpr uint64_t MAX_STEP_MS = 250U;

            GaitCycle(Bot::Legs::LegsInterface &legs, GaitParams &gaitParams);

            ~GaitCycle() = default;

            bool Start(void);

            bool Pause(void);

            bool Stop(void);

            bool SetGaitType(GaitType gaitType);

            void Update(uint64_t currentTime);

        private:
            void                      SelectStrategy(GaitType gaitType);

            void                      ApplyGait(float scale);

            Bot::Legs::LegsInterface &mLegs;
            GaitParams               &mGaitParams;
            GaitTripod                mGaitTripod;
            GaitWave                  mGaitWave;
            GaitRipple                mGaitRipple;
            GaitStrategy             *mGaitStrategy;

            uint64_t                  mLastUpdateTime;
            bool                      mHasLastUpdateTime;
            float                     mPhase;
            float                     mRamp;
            bool                      mResetPhaseWhenIdle;
            bool                      mSettled;

            // Foot goes through the swing (lift) then the stance (push), the extremities match.
            const GaitTrajectory      mTrajectory = {{{{{0.0F, 1.0F, 0.0F},
                                                        {0.0F, 0.0F, -2.0F},
                                                        {0.0F, -1.0F, 0.0F}}},
                                                      {{{0.0F, -1.0F, 0.0F},
                                                        {0.0F, 0.0F, 0.1F},
                                                        {0.0F, 1.0F, 0.0F}}}}};
        };
    } // namespace Gait
} // namespace Move

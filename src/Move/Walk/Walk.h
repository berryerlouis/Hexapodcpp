#pragma once
#include "Constants.h"
#include "../../Bot/Legs/LegsInterface.h"
#include "../Gait/GaitParams.h"
#include "../Gait/GaitCycle.h"

namespace Move
{
    namespace Walk
    {
        class Walk {
        public:
            Walk(Bot::Legs::LegsInterface &legs);

            ~Walk() = default;

            void UpdateStatus(const EWalkStatus status);

            EWalkStatus GetStatus(void) const;

            void Update(const uint64_t currentTime);

            Gait::GaitParams &GetParams(void);

            bool SetGait(Gait::GaitType gait);

            Gait::GaitType GetGait(void) const;

            bool SetCycleDuration(const uint16_t duration);

            uint16_t GetCycleDuration(void);

        private:
            Bot::Legs::LegsInterface &mLegs;
            EWalkStatus mStatus;
            EWalkStatus mPreviousStatus;
            Gait::GaitCycle mGaitCycle;
        };
    }
}

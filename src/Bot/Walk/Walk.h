#pragma once
#include "../Constants.h"
#include "../Legs/Legs.h"
#include "../Gait/GaitParams.h"
#include "../../Misc/Maths/Geometry.h"
#include "../../Driver/Tick/TickInterface.h"
#include "../Gait/GaitCycle.h"

namespace Bot
{
    namespace Walk
    {
        class Walk {
        public:
            Walk(Legs::Legs &legs, Driver::Tick::TickInterface &tick);

            ~Walk() = default;

            void UpdateStatus(const EWalkStatus status);

            EWalkStatus GetStatus(void) const;

            void Update(const uint64_t currentTime);

            Gait::GaitParams &GetParams(void);

            bool SetGait(Gait::GaitType gait);

            Gait::GaitType GetGait(void) const;

            bool SetCycleDuration(const uint16_t duration);

            uint16_t GetCycleDuration(void) const;

        private:
            void Play(const uint64_t currentTime);

            void Pause(const uint64_t currentTime);

            void Stop(const uint64_t currentTime);

            Legs::Legs &mLegs;
            Driver::Tick::TickInterface &mTick;
            EWalkStatus mStatus;
            EWalkStatus mPreviousStatus;
            Gait::GaitParams mParams;
            Gait::GaitCycle mGaitCycle;
        };
    }
}

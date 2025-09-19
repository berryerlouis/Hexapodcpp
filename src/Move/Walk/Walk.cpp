#include "Walk.h"

#include <cstdio>

#include "../../Misc/Maths/Utils.h"

// https://github.com/JakobLeander/hexapod/blob/main/hexapod/

namespace Move
{
    namespace Walk
    {

        Walk::Walk(Bot::Legs::LegsInterface &legs, Driver::Tick::TickInterface &tick) :
            mLegs(legs)
            , mTick(tick)
            , mStatus(STOP)
            , mPreviousStatus(STOP)
            , mGaitCycle(mLegs, Gait::RIPPLE, mTick) {
            LOG_MOVE_DEBUG("Walk", "Initialized.");
        }

        void Walk::UpdateStatus(const EWalkStatus status) {
            this->mStatus = status;
            LOG_MOVE_DEBUG("Walk", "Update Status: %s.", WalkStatusToString(this->mStatus).c_str());
        }

        EWalkStatus Walk::GetStatus(void) const {
            return this->mStatus;
        }

        void Walk::Update(const uint64_t currentTime) {
            if (this->mPreviousStatus != this->mStatus) {
                switch (this->mStatus) {
                    case PLAY:
                        this->mGaitCycle.Start();
                        break;
                    case PAUSE:
                        this->mGaitCycle.Pause();
                        break;
                    case STOP:
                        this->mGaitCycle.Stop();
                        break;
                }
                this->mPreviousStatus = this->mStatus;
            }
            this->mGaitCycle.Update(currentTime);
        }

        Gait::GaitParams &Walk::GetParams() {
            return this->mGaitCycle.GetGaitParams();
        }

        bool Walk::SetGait(const Gait::GaitType gait) {
            this->mGaitCycle.SetGaitType(gait);
            return this->GetGait();
        }

        Gait::GaitType Walk::GetGait(void) const {
            return this->mGaitCycle.GetGaitType();
        }

        bool Walk::SetCycleDuration(const uint16_t duration) {
            const bool success = this->mGaitCycle.GetGaitParams().SetCycleDuration(duration);
            this->mGaitCycle.ResetCycleStep();
            return success;
        }

        uint16_t Walk::GetCycleDuration(void) {
            return this->mGaitCycle.GetGaitParams().GetCycleDuration();
        }
    }

}

#include "Walk.h"

#include <cstdio>

#include "../../Misc/Maths/Utils.h"

namespace Bot
{
    namespace Walk
    {
#define AMPLITUDE_MAX   3.0F
#define ELEVATION_MAX   3.0F

        Walk::Walk(Legs::Legs &legs, Driver::Tick::TickInterface &tick) :
            mLegs(legs)
            , mTick(tick)
            , mStatus(STOP)
            , mPreviousStatus(STOP)
            , mParams(tick, 0.0F, AMPLITUDE_MAX, ELEVATION_MAX, 0.0F)
            , mGaitCycle(legs, mParams, Gait::RIPPLE, tick) {
            LOG_BOT_DEBUG("Walk", "Initialized.");
        }

        void Walk::UpdateStatus(const EWalkStatus status) {
            this->mStatus = status;
            LOG_BOT_INFO("Walk", "Update Status: %s.", WalkStatusToString(this->mStatus).c_str());
        }

        EWalkStatus Walk::GetStatus(void) const {
            return this->mStatus;
        }

        void Walk::Update(const uint64_t currentTime) {
            if (this->mPreviousStatus != this->mStatus) {
                switch (this->mStatus) {
                    case PLAY:
                        this->Play(currentTime);
                        break;
                    case PAUSE:
                        this->Pause(currentTime);
                        break;
                    case STOP:
                        this->Stop(currentTime);
                        break;
                }
                this->mPreviousStatus = this->mStatus;
            }
            this->mGaitCycle.Update(currentTime);
        }

        void Walk::Play(const uint64_t currentTime) {
            (void) currentTime;
            this->mGaitCycle.Start();
        }

        void Walk::Pause(const uint64_t currentTime) {
            (void) currentTime;
            this->mGaitCycle.Pause();
        }

        void Walk::Stop(const uint64_t currentTime) {
            (void) currentTime;
            this->mGaitCycle.Stop();
        }

        Gait::GaitParams &Walk::GetParams() {
            return this->mParams;
        }

        bool Walk::SetGait(const Gait::GaitType gait) {
            this->mGaitCycle.SetGaitType(gait);
            return this->GetGait();
        }

        Gait::GaitType Walk::GetGait(void) const {
            return this->mGaitCycle.GetGaitType();
        }

        bool Walk::SetCycleDuration(const uint16_t duration) {
            const bool success = this->mParams.SetCycleDuration(duration);
            this->mGaitCycle.ResetCycleStep();
            return success;
        }

        uint16_t Walk::GetCycleDuration(void) const {
            return this->mParams.GetCycleDuration();
        }
    }

}

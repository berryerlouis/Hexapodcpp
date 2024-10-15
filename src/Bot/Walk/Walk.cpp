#include "Walk.h"

namespace Bot
{
    namespace Walk
    {
        Walk::Walk(Legs::Legs &legs)
            : mLegs(legs)
              , mStatus(PAUSE)
              , mCounterStepPosition(0U)
              , mMaxCounterStepPosition(0U)
              , mWalkStepPosition{}
              , mDelayStep(0U)
              , mPreviousTime(0U)
              , mStepFinished(false) {
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                for (size_t steps = 0U; steps < 10U; steps++) {
                    this->mWalkStepPosition[legId].positions[steps].x = 0;
                    this->mWalkStepPosition[legId].positions[steps].y = 0;
                    this->mWalkStepPosition[legId].positions[steps].z = 0;
                    this->mWalkStepPosition[legId].idxPosition = 0;
                }
            }

            this->mWalkStepPosition[FRONT_LEFT].positions[0].x = -20;
            this->mWalkStepPosition[FRONT_LEFT].positions[0].y = 0;
            this->mWalkStepPosition[FRONT_LEFT].positions[0].z = 0;
            this->mWalkStepPosition[FRONT_LEFT].delayStep = 1000;
            this->mWalkStepPosition[FRONT_LEFT].positions[1].x = 20;
            this->mWalkStepPosition[FRONT_LEFT].positions[1].y = 0;
            this->mWalkStepPosition[FRONT_LEFT].positions[1].z = 0;
            this->mWalkStepPosition[FRONT_LEFT].delayStep = 1000;
            this->mWalkStepPosition[FRONT_LEFT].positions[2].x = 0;
            this->mWalkStepPosition[FRONT_LEFT].positions[2].y = 0;
            this->mWalkStepPosition[FRONT_LEFT].positions[2].z = 20;
            this->mWalkStepPosition[FRONT_LEFT].delayStep = 1000;

            this->mMaxCounterStepPosition = 3U;
        }

        void Walk::UpdateStatus(const EWalkStatus status) {
            this->mStatus = status;
        }

        void Walk::Update(const uint64_t currentTime, const Position3d &bodyIk) {
            switch (this->mStatus) {
                case PLAY:
                    this->Play(currentTime, bodyIk);
                    break;
                case PAUSE:
                    this->Pause(currentTime, bodyIk);
                    break;
                case STOP:
                    this->Stop(currentTime, bodyIk);
                    break;
            }
        }

        void Walk::Play(const uint64_t currentTime, const Position3d &bodyIk) {
            //for (size_t legId = 0U; legId < NB_LEGS; legId++) {
            size_t legId = 0U;
            Leg::Leg leg = this->mLegs.GetLeg(legId);
            Step &step = this->mWalkStepPosition[legId];

            //time to do a step
            if (currentTime > step.startTime + step.delayStep) {
                step.startTime = currentTime;
                leg.SetLegIk(step.positions[step.idxPosition],
                             bodyIk, step.delayStep);
                if (step.idxPosition < this->mMaxCounterStepPosition) {
                    step.idxPosition++;
                } else {
                    step.idxPosition = 0U;
                }
            }
            //}
        }

        void Walk::Pause(const uint64_t currentTime, const Position3d &bodyIk) {
            (void) currentTime;
            (void) bodyIk;
        }

        void Walk::Stop(const uint64_t currentTime, const Position3d &bodyIk) {
            (void) currentTime;
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
                leg.SetLegIk({.x = 0, .y = 0, .z = 0}, bodyIk, 1000U);
            }
            this->mCounterStepPosition = 0U;
            this->UpdateStatus(PAUSE);
        }
    }
}

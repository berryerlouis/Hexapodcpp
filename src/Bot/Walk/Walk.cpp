#include "Walk.h"

namespace Bot
{
    namespace Walk
    {
        Walk::Walk(Legs::Legs &legs) :
            mLegs(legs)
            , mStatus(PAUSE)
            , mCounterStepPosition(0U)
            , mMaxCounterStepPosition(0U)
            , mWalkStepPosition{}
            , mDelayStep(0U)
            , mPreviousTime(0U)
            , mStepFinished(false) {
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                for (size_t steps = 0U; steps < 4U; steps++) {
                    this->mWalkStepPosition[legId].positions[steps].x = 0;
                    this->mWalkStepPosition[legId].positions[steps].y = 0;
                    this->mWalkStepPosition[legId].positions[steps].z = 0;
                    this->mWalkStepPosition[legId].idxPosition = 0;
                }
            }

            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                this->mWalkStepPosition[legId].positions[0U] = {0, -3, 0};
                this->mWalkStepPosition[legId].delayStep = 1000U;
                this->mWalkStepPosition[legId].positions[1U] = {0, 3, -1};
                this->mWalkStepPosition[legId].delayStep = 1000U;
                this->mWalkStepPosition[legId].positions[2U] = {0, 3, 0};
                this->mWalkStepPosition[legId].delayStep = 1000U;
            }

            this->mMaxCounterStepPosition = 2U;
        }

        void Walk::UpdateStatus(const EWalkStatus status) {
            this->mStatus = status;
        }

        void Walk::Update(const uint64_t currentTime) {
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
        }

        void Walk::Play(const uint64_t currentTime) {
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
                Step &step = this->mWalkStepPosition[legId];

                //time to do a step
                if (currentTime > step.startTime + step.delayStep) {
                    step.startTime = currentTime;
                    uint8_t next = (step.idxPosition + 2U) % 3U;
                    leg.SetLegIk(step.positions[legId % 2U ? step.idxPosition : next], step.delayStep);
                    if (step.idxPosition < this->mMaxCounterStepPosition) {
                        step.idxPosition++;
                    } else {
                        step.idxPosition = 0U;
                    }
                }
            }
        }

        void Walk::Pause(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Walk::Stop(const uint64_t currentTime) {
            (void) currentTime;
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
                leg.SetLegIk({0, 0, 0}, 1000U);
            }
            this->mCounterStepPosition = 0U;
            this->UpdateStatus(PAUSE);
        }
    }
}

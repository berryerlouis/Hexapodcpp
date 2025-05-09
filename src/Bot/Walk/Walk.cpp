#include "Walk.h"

#include <cstdio>

#include "../../Misc/Maths/Utils.h"

namespace Bot
{
    namespace Walk
    {
#define AMPLITUDE_MAX   3.0F
#define ELEVATION_MAX   3.0F

        Walk::Walk(Legs::Legs &legs) :
            mLegs(legs)
            , mStatus(PAUSE)
            , mCounterStepPosition(0U)
            , mMaxCounterStepPosition(0U)
            , mWalkStepPosition{}
            , mDelayStep(0U)
            , mPreviousTime(0U)
            , mStepFinished(false)
            , mDirection(0U)
            , mAmplitude(AMPLITUDE_MAX)
            , mElevation(ELEVATION_MAX) {
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                for (size_t steps = 0U; steps < NB_STEPS; steps++) {
                    this->mWalkStepPosition[legId].steps[steps].positions.x = 0;
                    this->mWalkStepPosition[legId].steps[steps].positions.y = 0;
                    this->mWalkStepPosition[legId].steps[steps].positions.z = 0;
                    this->mWalkStepPosition[legId].idxPosition = 0;
                }
            }

            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                if (legId % 2U == 0U) {
                    this->mWalkStepPosition[legId].steps[0U].positions = {0, -1, 0};
                    this->mWalkStepPosition[legId].steps[0U].delayStep = 500U;
                    this->mWalkStepPosition[legId].steps[1U].positions = {0, 0, -1};
                    this->mWalkStepPosition[legId].steps[1U].delayStep = 500U;
                    this->mWalkStepPosition[legId].steps[2U].positions = {0, 1, 0};
                    this->mWalkStepPosition[legId].steps[2U].delayStep = 1000U;
                } else {
                    this->mWalkStepPosition[legId].steps[1U].positions = {0, 1, 0};
                    this->mWalkStepPosition[legId].steps[1U].delayStep = 1000U;
                    this->mWalkStepPosition[legId].steps[2U].positions = {0, -1, 0};
                    this->mWalkStepPosition[legId].steps[2U].delayStep = 500U;
                    this->mWalkStepPosition[legId].steps[0U].positions = {0, 0, -1};
                    this->mWalkStepPosition[legId].steps[0U].delayStep = 500U;
                }
            }

            this->mMaxCounterStepPosition = 3U;
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

        bool Walk::SetDirection(const uint16_t directionAngle) {
            if (directionAngle <= 360U) {
                this->mDirection = directionAngle * M_PI / 180U;
                return true;
            }
            return false;
        }

        bool Walk::SetAmplitudeElevation(const uint16_t amplitude, const uint16_t elevation) {
            if ((amplitude <= AMPLITUDE_MAX * 10.0F) && (elevation <= ELEVATION_MAX * 10.0F)) {
                this->mAmplitude = amplitude / 10.0F;
                this->mElevation = elevation / 10.0F;
                return true;
            }
            return false;
        }

        uint16_t Walk::GetDirection(void) const {
            return this->mDirection * 180.0F / M_PI;
        }

        uint8_t Walk::GetAmplitude(void) const {
            return this->mAmplitude;
        }

        uint8_t Walk::GetElevation(void) const {
            return this->mElevation;
        }

        void Walk::Play(const uint64_t currentTime) {
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
                Steps &legSteps = this->mWalkStepPosition[legId];

                //time to do a step
                if (currentTime >= legSteps.startTime + legSteps.steps[legSteps.idxPosition].delayStep) {
                    legSteps.startTime = currentTime;
                    if (legSteps.idxPosition < this->mMaxCounterStepPosition) {
                        // Move to next position
                        legSteps.idxPosition++;
                    } else {
                        legSteps.idxPosition = 0U;
                    }
                } else {
                    const float deltaTime = (legSteps.steps[legSteps.idxPosition].delayStep
                                             - (legSteps.steps[legSteps.idxPosition].delayStep
                                                - (currentTime - legSteps.startTime))) /
                                            legSteps.steps[legSteps.idxPosition].delayStep;
                    Position3d posCurrent = legSteps.steps[legSteps.idxPosition].positions;
                    const uint8_t next = (legSteps.idxPosition + 1U) % NB_STEPS;
                    Position3d posNext = legSteps.steps[next].positions;

                    if (legId < NB_LEGS / 2U) {
                        posCurrent.x = this->mAmplitude * (legSteps.steps[legSteps.idxPosition].positions.x *
                                                           cos(this->mDirection)) +
                                       this->mAmplitude * (legSteps.steps[legSteps.idxPosition].positions.y *
                                                           sin(this->mDirection));
                        posCurrent.y = this->mAmplitude * (legSteps.steps[legSteps.idxPosition].positions.y *
                                                           cos(this->mDirection)) -
                                       this->mAmplitude * (legSteps.steps[legSteps.idxPosition].positions.x *
                                                           sin(this->mDirection));
                        posNext.x = this->mAmplitude * (legSteps.steps[next].positions.x * cos(this->mDirection)) +
                                    this->mAmplitude * (legSteps.steps[next].positions.y * sin(this->mDirection));
                        posNext.y = this->mAmplitude * (legSteps.steps[next].positions.y * cos(this->mDirection)) -
                                    this->mAmplitude * (legSteps.steps[next].positions.x * sin(this->mDirection));

                    } else {
                        posCurrent.x = this->mAmplitude * (legSteps.steps[legSteps.idxPosition].positions.x *
                                                           cos(this->mDirection)) -
                                       this->mAmplitude * (legSteps.steps[legSteps.idxPosition].positions.y *
                                                           sin(this->mDirection));
                        posCurrent.y = this->mAmplitude * (legSteps.steps[legSteps.idxPosition].positions.y *
                                                           cos(this->mDirection)) +
                                       this->mAmplitude * (legSteps.steps[legSteps.idxPosition].positions.x *
                                                           sin(this->mDirection));
                        posNext.x = this->mAmplitude * (legSteps.steps[next].positions.x * cos(this->mDirection)) -
                                    this->mAmplitude * (legSteps.steps[next].positions.y * sin(this->mDirection));
                        posNext.y = this->mAmplitude * (legSteps.steps[next].positions.y * cos(this->mDirection)) +
                                    this->mAmplitude * (legSteps.steps[next].positions.x * sin(this->mDirection));
                    }
                    posCurrent.z = this->mElevation * legSteps.steps[legSteps.idxPosition].positions.z;
                    posNext.z = this->mElevation * legSteps.steps[next].positions.z;

                    if (legId >= NB_LEGS / 2U) {
                        posCurrent.y *= -1.0F;
                        posNext.y *= -1.0F;
                    }
                    const Position3d pos = {
                            Misc::Utils::LerpF(posCurrent.x, posNext.x, deltaTime),
                            Misc::Utils::LerpF(posCurrent.y, posNext.y, deltaTime),
                            Misc::Utils::LerpF(posCurrent.z, posNext.z, deltaTime),
                    };

                    leg.SetLegIk(pos, 0);
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

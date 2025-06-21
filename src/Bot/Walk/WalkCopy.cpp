#include "Walk.h"

#include <cstdio>

#include "../../Misc/Maths/Utils.h"

namespace Bot
{
    namespace WalkCpy
    {
#define AMPLITUDE_MAX   3.0F
#define ELEVATION_MAX   3.0F

        WalkCpy::WalkCpy(Legs::Legs &legs, Driver::Tick::TickInterface &tick) :
            mLegs(legs)
            , mTick(tick)
            , mStatus(PAUSE) {

            /*, mMaxCounterStepPosition(0U)
            , mWalkStepPosition{}
            , mDelayStep(0U)
            , mPreviousTime(0U)
            , mStepFinished(false)
            , mStartTimeDirection(0U)
            , mDirection(0.0F)
            , mTargetDirection(0.0F)
            , mStartTimeAmplitude(0U)
            , mStartTimeElevation(0U)
            , mAmplitude(AMPLITUDE_MAX)
            , mTargetAmplitude(AMPLITUDE_MAX)
            , mElevation(ELEVATION_MAX)
            , mTargetElevation(ELEVATION_MAX) {
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                for (size_t steps = 0U; steps < NB_STEPS; steps++) {
                    this->mWalkStepPosition[legId].steps[steps].positions.x = 0.0F;
                    this->mWalkStepPosition[legId].steps[steps].positions.y = 0.0F;
                    this->mWalkStepPosition[legId].steps[steps].positions.z = 0.0F;
                    this->mWalkStepPosition[legId].idxPosition = 0U;
                }
            }

            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                if (legId % 2U == 0U) {
                    this->mWalkStepPosition[legId].steps[0U].positions = {0.0F, -1.0F, 0.0F};
                    this->mWalkStepPosition[legId].steps[0U].delayStep = 1000U;
                    this->mWalkStepPosition[legId].steps[0U].onGround = true;
                    this->mWalkStepPosition[legId].steps[1U].positions = {0.0F, 0.0F, -1.0F};
                    this->mWalkStepPosition[legId].steps[1U].delayStep = 1000U;
                    this->mWalkStepPosition[legId].steps[0U].onGround = false;
                    this->mWalkStepPosition[legId].steps[2U].positions = {0.0F, 1.0F, 0.0F};
                    this->mWalkStepPosition[legId].steps[2U].delayStep = 1000U;
                    this->mWalkStepPosition[legId].steps[0U].onGround = false;
                } else {
                    this->mWalkStepPosition[legId].steps[1U].positions = {0.0F, 1.0F, 0.0F};
                    this->mWalkStepPosition[legId].steps[1U].delayStep = 1000U;
                    this->mWalkStepPosition[legId].steps[0U].onGround = false;
                    this->mWalkStepPosition[legId].steps[2U].positions = {0.0F, -1.0F, 0.0F};
                    this->mWalkStepPosition[legId].steps[2U].delayStep = 1000U;
                    this->mWalkStepPosition[legId].steps[0U].onGround = true;
                    this->mWalkStepPosition[legId].steps[0U].positions = {0.0F, 0.0F, -1.0F};
                    this->mWalkStepPosition[legId].steps[0U].delayStep = 1000U;
                    this->mWalkStepPosition[legId].steps[0U].onGround = false;
                }
            }
            this->mMaxCounterStepPosition = 3U;
        }
    */
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
            /*for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
                Steps &legSteps = this->mWalkStepPosition[legId];

                //time to do a step
                if (currentTime >= legSteps.startTime + legSteps.steps[legSteps.idxPosition].delayStep) {
                    this->NextStep(currentTime, legSteps);
                } else {
                    const float deltaTime = GetDeltaTime(legSteps.steps[legSteps.idxPosition].delayStep,
                                                         currentTime,
                                                         legSteps.startTime);
                    Position3d newPosition = this->ComputeNewLegPosition(deltaTime, legSteps, legId);
                    leg.SetLegIk(newPosition);
                }
            }
            this->UpdateDirection(currentTime);
            this->UpdateAmplitude(currentTime);
            this->UpdateElevation(currentTime);*/
        }

        void Walk::Pause(const uint64_t currentTime) {
            (void) currentTime;
            this->UpdateDirection(currentTime);
            this->UpdateAmplitude(currentTime);
            this->UpdateElevation(currentTime);
        }

        void Walk::Stop(const uint64_t currentTime) {
            (void) currentTime;
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
                leg.SetLegIk({0, 0, 0}, 1000U);
            }
            this->UpdateStatus(PAUSE);
        }


        bool Walk::ApplyDirection(const uint16_t directionAngle) {
            if (directionAngle <= 360U) {
                this->mTargetDirection = directionAngle * M_PI / 180U;
                this->mStartTimeDirection = this->mTick.GetMs();
                return true;
            }
            return false;
        }

        bool Walk::ApplyAmplitude(const uint8_t amplitude) {
            if (amplitude <= AMPLITUDE_MAX * 10.0F) {
                this->mTargetAmplitude = amplitude / 10.0F;
                this->mStartTimeAmplitude = this->mTick.GetMs();
                return true;
            }
            return false;
        }

        bool Walk::ApplyElevation(const uint8_t elevation) {
            if (elevation <= ELEVATION_MAX * 10.0F) {
                this->mTargetElevation = elevation / 10.0F;
                this->mStartTimeElevation = this->mTick.GetMs();
                return true;
            }
            return false;
        }

        uint16_t Walk::GetDirection(void) const {
            return this->mTargetDirection * 180.0F / M_PI;
        }

        uint8_t Walk::GetAmplitude(void) const {
            return this->mTargetAmplitude * 10.0F;
        }

        uint8_t Walk::GetElevation(void) const {
            return this->mTargetElevation * 10.0F;
        }

        void Walk::SetAmplitude(Position3d &position) const {
            position.x *= this->mAmplitude;
            position.y *= this->mAmplitude;
        }

        void Walk::SetElevation(Position3d &position) const {
            position.z *= this->mElevation;
        }

        void Walk::UpdateDirection(const uint64_t currentTime) {
            (void) currentTime;
            if (currentTime <= this->mStartTimeDirection + 1000U) {
                const float deltaTime = GetDeltaTime(1000U,
                                                     currentTime,
                                                     this->mStartTimeDirection);
                this->mDirection = Misc::Utils::LerpF(this->mDirection, this->mTargetDirection, deltaTime);
            } else {
                this->mDirection = this->mTargetDirection;
            }


            if (currentTime <= this->mStartTimeDirection + 1000U) {
                const float deltaTime = GetDeltaTime(1000U, currentTime, this->mStartTimeDirection);
                float deltaAngle = this->mTargetDirection - this->mDirection;
                if (deltaAngle > M_PI)
                    deltaAngle -= 2.0F * M_PI;
                if (deltaAngle < -M_PI)
                    deltaAngle += 2.0F * M_PI;

                this->mDirection += deltaAngle * deltaTime;
            } else {
                this->mDirection = this->mTargetDirection;
            }
        }

        void Walk::UpdateAmplitude(const uint64_t currentTime) {
            if (currentTime <= this->mStartTimeAmplitude + 1000U) {
                const float deltaTime = GetDeltaTime(1000U, currentTime, this->mStartTimeAmplitude);
                this->mAmplitude = Misc::Utils::LerpF(this->mAmplitude, this->mTargetAmplitude, deltaTime);
            } else {
                this->mAmplitude = this->mTargetAmplitude;
            }
        }

        void Walk::UpdateElevation(const uint64_t currentTime) {
            if (currentTime <= this->mStartTimeElevation + 1000U) {
                const float deltaTime = GetDeltaTime(1000U, currentTime, this->mStartTimeElevation);
                this->mElevation = Misc::Utils::LerpF(this->mElevation, this->mTargetElevation, deltaTime);
            } else {
                this->mElevation = this->mTargetElevation;
            }
        }

        Position3d Walk::ComputeNewLegPosition(const float deltaTime, const Steps &legSteps,
                                               const uint8_t legId) const {
            // get next position index to compute lerp
            const uint8_t next = (legSteps.idxPosition + 1U) % NB_STEPS;

            // apply rotation
            Position3d posCurrent = Rotate(this->mDirection, legSteps.steps[legSteps.idxPosition].positions,
                                           (legId < NB_LEGS / 2U));
            Position3d posNext = Rotate(this->mDirection, legSteps.steps[next].positions,
                                        (legId < NB_LEGS / 2U));

            // apply step amplitude
            SetAmplitude(posCurrent);
            SetAmplitude(posNext);
            // apply step elevation
            SetElevation(posCurrent);
            SetElevation(posNext);

            return {
                    Misc::Utils::LerpF(posCurrent.x, posNext.x, deltaTime),
                    Misc::Utils::LerpF(posCurrent.y, posNext.y, deltaTime),
                    Misc::Utils::LerpF(posCurrent.z, posNext.z, deltaTime),
            };
        }

        void Walk::NextStep(const uint64_t currentTime, Steps &legSteps) const {
            legSteps.startTime = currentTime;
            if (legSteps.idxPosition < this->mMaxCounterStepPosition) {
                // Move to next position
                legSteps.idxPosition++;
            } else {
                legSteps.idxPosition = 0U;
            }
        }

        float Walk::GetDeltaTime(const float step, const uint64_t currentTime, const uint64_t startTime) {
            return (step - (step - (currentTime - startTime))) / step;
        }

        Position3d Walk::Rotate(const float angle, const Position3d &position, const bool clockWize) {
            Position3d rotatePosition = position;
            const float clockwize = clockWize ? 1.0F : -1.0F;
            const float newAngle = clockWize ? angle : -angle - M_PI;
            rotatePosition.x = (position.x * cos(newAngle)) + (clockwize * position.y * sin(newAngle));
            rotatePosition.y = (position.y * cos(newAngle)) - (clockwize * position.x * sin(newAngle));
            return rotatePosition;
        }
    }

}

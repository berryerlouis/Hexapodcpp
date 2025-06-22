#include "GaitCycle.h"

#include <iostream>

namespace Bot
{
    namespace Gait
    {
        GaitCycle::GaitCycle(Legs::Legs &legs, GaitParams &params, const GaitType gait,
                             Driver::Tick::TickInterface &tick) :
            mLegs(legs)
            , mParams(params)
            , mTick(tick)
            , mIsRunning(false)
            , mGaits(params, gait) {
        }

        GaitBase &GaitCycle::GetSelectedGait(void) const {
            return this->mGaits.GetSelectedGait();
        }

        bool GaitCycle::SetGaitType(const GaitType gaitType) {
            this->mGaits.SetGaitType(gaitType);
            if (this->mIsRunning == true) {
                this->Pause();
                this->Start();
            }
            return true;
        }

        GaitType GaitCycle::GetGaitType(void) const {
            return this->mGaits.GetGaitType();
        }

        bool GaitCycle::Start(void) {
            if (this->mIsRunning == false) {
                this->mIsRunning = true;
                this->ResetCycleStep();
                return true;
            }
            return false;
        }

        bool GaitCycle::Pause(void) {
            if (this->mIsRunning == true) {
                this->mIsRunning = false;
                return true;
            }
            return false;
        }

        bool GaitCycle::Stop(void) {
            if (this->mIsRunning == true) {
                this->mIsRunning = false;
                for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                    Leg::Leg *leg = this->mLegs.GetLeg(legId);
                    leg->SetLegIk({0, 0, 0}, this->mParams.GetCycleDuration());
                }
                return true;
            }
            return false;
        }

        void GaitCycle::Update(const uint64_t currentTime) const {
            if (this->mIsRunning == true) {
                this->UpdateCycle(currentTime);
            }
            this->mParams.Update(currentTime);
        }

        void GaitCycle::ResetCycleStep(void) const {
            const uint64_t startTime = this->mTick.GetMs();
            for (uint8_t groupId = 0U; groupId < this->GetSelectedGait().GetGroups().size(); ++groupId) {
                this->GetSelectedGait().ResetCycleStep(groupId, startTime);
            }
        }

        void GaitCycle::UpdateCycle(const uint64_t currentTime) const {
            for (uint8_t groupId = 0U; groupId < this->GetSelectedGait().GetGroups().size(); ++groupId) {
                if (this->GetSelectedGait().CanUpdate(currentTime, groupId) == true) {
                    this->GetSelectedGait().Update(currentTime, groupId);
                    const float deltaTime = this->GetSelectedGait().GetDeltaTime(currentTime, groupId);
                    const uint8_t posId = this->GetSelectedGait().GetCurrentIndex(groupId);
                    for (const ELeg legId: this->GetSelectedGait().GetGroupLeg(groupId)) {
                        this->SetLegTarget(legId, posId, deltaTime);
                        (void) this->mLegs.GetLeg(legId)->UpdatePosition(deltaTime);
                    }
                }
            }
        }

        void GaitCycle::SetLegTarget(const uint8_t legId, const uint8_t posId, const float deltaTime) const {
            Leg::Leg *leg = this->mLegs.GetLeg(legId);
            Misc::Maths::Position3d position = this->GetSelectedGait().GetPosition(posId);
            Misc::Maths::Position3d nextPosition = this->GetSelectedGait().GetPosition(
                    (posId + 1U) % this->GetSelectedGait().GetNbSteps());

            leg->ComputeDirection(position, this->mParams.GetCurrentDirection(),
                                  (legId < NB_LEGS / 2U));
            leg->ComputeAmplitude(position, this->mParams.GetCurrentAmplitude());
            leg->ComputeElevation(position, this->mParams.GetCurrentElevation());

            leg->ComputeDirection(nextPosition, this->mParams.GetCurrentDirection(),
                                  (legId < NB_LEGS / 2U));
            leg->ComputeAmplitude(nextPosition, this->mParams.GetCurrentAmplitude());
            leg->ComputeElevation(nextPosition, this->mParams.GetCurrentElevation());
            position = Misc::Utils::LerpF3d(position, nextPosition, deltaTime / 1.5F);

            leg->SetTarget(position);
        }
    }

}

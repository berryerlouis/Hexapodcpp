#include "GaitCycle.h"

namespace Move
{
    namespace Gait
    {
#define AMPLITUDE_MAX   3.0F
#define ELEVATION_MAX   3.0F

        GaitCycle::GaitCycle(Bot::Legs::LegsInterface &legs, const GaitType gait) :
            mLegs(legs)
            , mParams(0.0F, AMPLITUDE_MAX, ELEVATION_MAX, 0.0F)
            , mIsRunning(false)
            , mGaits(mParams, gait) {
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

        GaitParams &GaitCycle::GetGaitParams() {
            return this->mParams;
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
                for (uint8_t legId = 0U; legId < 6U; legId++) {
                    Bot::Leg::LegInterface *leg = this->mLegs.GetLeg(static_cast<Bot::Legs::ELeg>(legId));
                    leg->ResetTarget();
                    leg->SetLegIk({0.0F, 0.0F, 0.0F}, this->mParams.GetCycleDuration());
                }
                return true;
            }
            return false;
        }

        void GaitCycle::Update(const uint64_t currentTime) {
            if (this->mIsRunning == true) {
                this->UpdateCycle(currentTime);
            }
            this->mParams.Update(currentTime);
        }

        void GaitCycle::ResetCycleStep(void) const {
            const uint64_t startTime = Driver::Tick::Tick::GetInstance().GetMs();
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
                    for (const Bot::Legs::ELeg legId: this->GetSelectedGait().GetGroupLeg(groupId)) {
                        this->SetLegTarget(legId, posId, deltaTime);
                        (void) this->mLegs.GetLeg(legId)->UpdatePosition(deltaTime);
                    }
                }
            }
        }

        void GaitCycle::SetLegTarget(const Bot::Legs::ELeg legId, const uint8_t posId, const float deltaTime) const {
            Bot::Leg::LegInterface *leg = this->mLegs.GetLeg(legId);
            Misc::Maths::Position3d position = this->GetSelectedGait().GetPosition(posId);
            Misc::Maths::Position3d nextPosition = this->GetSelectedGait().GetPosition(
                    (posId + 1U) % this->GetSelectedGait().GetNbSteps());

            if (false == this->mParams.IsRotated()) {
                leg->ComputeDirection(position, this->mParams.GetCurrentDirection());
                leg->ComputeDirection(nextPosition, this->mParams.GetCurrentDirection());
            } else {
                leg->ComputeRotation(position, this->mParams.GetCurrentRotation(),
                                     this->mParams.GetCurrentRotationClockWize());
                leg->ComputeRotation(nextPosition, this->mParams.GetCurrentRotation(),
                                     this->mParams.GetCurrentRotationClockWize());
            }

            leg->ComputeAmplitude(position, this->mParams.GetCurrentAmplitude());
            leg->ComputeAmplitude(nextPosition, this->mParams.GetCurrentAmplitude());

            leg->ComputeElevation(position, this->mParams.GetCurrentElevation());
            leg->ComputeElevation(nextPosition, this->mParams.GetCurrentElevation());

            position = Misc::Utils::LerpF3d(position, nextPosition, deltaTime / 1.5F);

            leg->SetTarget(position);
        }
    }

}

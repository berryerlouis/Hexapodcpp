#include "Walk.h"

namespace Move
{
    namespace Walk
    {
        Walk::Walk(Bot::Legs::LegsInterface &legs)
            : mGaitParams()
            , mGaitCycle(legs, mGaitParams)
            , mStatus(EWalkStatus::STOPPED) {
        }

        void Walk::Update(const uint64_t currentTime) {
            this->mGaitCycle.Update(currentTime);
        }

        void Walk::UpdateStatus(const EWalkStatus status) {
            if (this->mStatus == status) {
                return;
            }

            this->mStatus = status;
            if (this->mStatus == EWalkStatus::WALKING) {
                this->mGaitCycle.Start();
            } else if (this->mStatus == EWalkStatus::PAUSED) {
                this->mGaitCycle.Pause();
            } else {
                this->mGaitCycle.Stop();
            }
        }

        EWalkStatus Walk::GetStatus() const {
            return this->mStatus;
        }

        bool Walk::SetDirection(const float directionAngle) {
            return this->mGaitParams.SetDirection(directionAngle);
        }

        bool Walk::SetRotation(const float rotationAngle, const bool clockWize) {
            return this->mGaitParams.SetRotation(rotationAngle, clockWize);
        }

        bool Walk::SetAmplitude(const float amplitude) {
            return this->mGaitParams.SetAmplitude(amplitude);
        }

        bool Walk::SetElevation(const float elevation) {
            return this->mGaitParams.SetElevation(elevation);
        }

        bool Walk::SetCycleDuration(const uint16_t duration) {
            return this->mGaitParams.SetCycleDuration(duration);
        }

        bool Walk::SetGait(const Gait::GaitType gait) {
            return this->mGaitCycle.SetGaitType(gait);
        }

        float Walk::GetDirection() const {
            return this->mGaitParams.GetDirection();
        }

        float Walk::GetRotation() const {
            return this->mGaitParams.GetRotation();
        }

        bool Walk::GetRotationClockWize() const {
            return this->mGaitParams.GetRotationClockWize();
        }

        bool Walk::IsRotated() const {
            return this->mGaitParams.IsRotated();
        }

        float Walk::GetAmplitude() const {
            return this->mGaitParams.GetAmplitude();
        }

        float Walk::GetElevation() const {
            return this->mGaitParams.GetElevation();
        }

        uint16_t Walk::GetCycleDuration() const {
            return this->mGaitParams.GetCycleDuration();
        }

        Gait::GaitType Walk::GetGait() const {
            return this->mGaitParams.GetGaitType();
        }
    } // namespace Walk
} // namespace Move

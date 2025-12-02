#pragma once

#include "../Gait/GaitCycle.h"
#include "../Gait/Constants.h"
#include "Constants.h"
#include "../../Bot/Legs/LegsInterface.h"

namespace Move
{
    namespace Walk
    {
        class Walk {
        public:
            Walk(Bot::Legs::LegsInterface &legs) :
                                                 mGaitParams()
                                                 , mGaitCycle(legs, mGaitParams)
                                                 , mStatus(EWalkStatus::STOPPED) {
            }

            ~Walk() = default;

            void
            Update(const uint64_t currentTime) {
                this->mGaitCycle.Update(currentTime);
            }

            void
            UpdateStatus(const EWalkStatus status) {
                this->mStatus = status;
                if (this->mStatus == EWalkStatus::WALKING) {
                    this->mGaitCycle.Start();
                } else if (this->mStatus == EWalkStatus::PAUSED) {
                    this->mGaitCycle.Pause();
                } else {
                    this->mGaitCycle.Stop();
                }
            }

            EWalkStatus
            GetStatus() const {
                return this->mStatus;
            }

            // Direct access to gait parameters
            bool
            SetDirection(const float directionAngle) {
                return this->mGaitParams.SetDirection(directionAngle);
            }

            bool
            SetRotation(const float rotationAngle, const bool clockWize) {
                return this->mGaitParams.SetRotation(rotationAngle, clockWize);
            }

            bool
            SetAmplitude(const float amplitude) {
                return this->mGaitParams.SetAmplitude(amplitude);
            }

            bool
            SetElevation(const float elevation) {
                return this->mGaitParams.SetElevation(elevation);
            }

            bool
            SetCycleDuration(const uint16_t duration) {
                return this->mGaitParams.SetCycleDuration(duration);
            }

            bool
            SetGait(const Gait::GaitType gait) {
                return this->mGaitParams.SetGaitType(gait);
            }

            float
            GetDirection() const {
                return this->mGaitParams.GetDirection();
            }

            float
            GetRotation() const {
                return this->mGaitParams.GetRotation();
            }

            bool
            GetRotationClockWize() const {
                return this->mGaitParams.GetRotationClockWize();
            }

            bool
            IsRotated() const {
                return this->mGaitParams.IsRotated();
            }

            float
            GetAmplitude() const {
                return this->mGaitParams.GetAmplitude();
            }

            float
            GetElevation() const {
                return this->mGaitParams.GetElevation();
            }

            uint16_t
            GetCycleDuration() const {
                return this->mGaitParams.GetCycleDuration();
            }

            Gait::GaitType
            GetGait() const {
                return this->mGaitParams.GetGaitType();
            }

        private:
            Gait::GaitParams mGaitParams;
            Gait::GaitCycle mGaitCycle;
            EWalkStatus mStatus;
        };
    }
}
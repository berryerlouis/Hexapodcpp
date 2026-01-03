#pragma once

#include "../../Bot/Legs/LegsInterface.h"
#include "../Gait/Constants.h"
#include "../Gait/GaitCycle.h"
#include "Constants.h"

namespace Move
{
    namespace Walk
    {
        class Walk {
        public:
            explicit Walk(Bot::Legs::LegsInterface &legs);

            ~Walk() = default;

            void        Update(const uint64_t currentTime);

            void        UpdateStatus(const EWalkStatus status);

            EWalkStatus GetStatus() const;

            // Direct access to gait parameters
            bool  SetDirection(const float directionAngle);

            bool  SetRotation(const float rotationAngle, const bool clockWize);

            bool  SetAmplitude(const float amplitude);

            bool  SetElevation(const float elevation);

            bool  SetCycleDuration(const uint16_t duration);

            bool  SetGait(const Gait::GaitType gait);

            float GetDirection() const;

            float GetRotation() const;

            bool  GetRotationClockWize() const;

            bool  IsRotated() const;

            float GetAmplitude() const;

            float GetElevation() const;

            uint16_t       GetCycleDuration() const;

            Gait::GaitType GetGait() const;

        private:
            Gait::GaitParams mGaitParams;
            Gait::GaitCycle  mGaitCycle;
            EWalkStatus      mStatus;
        };
    } // namespace Walk
} // namespace Move

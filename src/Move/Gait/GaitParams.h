#pragma once

#include "../../Driver/Timer/Tick.h"
#include "Constants.h"

namespace Move
{
    namespace Gait
    {
        class GaitParams {
        public:
            GaitParams(void);

            ~GaitParams() = default;

            // Gait control
            bool     SetGaitType(GaitType gaitType);

            GaitType GetGaitType() const;

            bool     SetDirection(float directionAngle);

            bool     SetRotation(float rotationAngle, bool clockWize);

            bool     SetAmplitude(float amplitude);

            bool     SetElevation(float elevation);

            bool     SetCycleDuration(uint16_t duration);

            float    GetDirection() const;

            float    GetRotation() const;

            bool     IsRotated() const;

            bool     GetRotationClockWize() const;

            float    GetAmplitude() const;

            float    GetElevation() const;

            uint16_t GetCycleDuration() const;

            bool     IsRunning() const;

            bool     SetRunning(bool running);

            uint64_t GetUpdatedTimeStamp() const;

        private:
            GaitType mCurrentGait;
            bool     mIsRunning;
            float    mDirection;
            float    mRotation;
            bool     mIsRotated;
            bool     mClockWize;
            float    mAmplitude;
            float    mElevation;
            uint16_t mCycleDuration;
            uint64_t mUpdatedTimeStamp;
        };
    } // namespace Gait
} // namespace Move

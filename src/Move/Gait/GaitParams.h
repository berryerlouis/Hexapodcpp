#pragma once
#include "../../Misc/Maths/Utils.h"
#include "../../Driver/Tick/Tick.h"

namespace Move
{
    namespace Gait
    {
        class GaitParams {
        public:
            GaitParams(const float directionAngle, const float amplitude,
                       const float elevation, const float rotation);

            ~GaitParams() = default;

            void Update(const uint64_t currentTime);

            bool SetDirection(const float directionAngle);

            bool SetRotation(const float rotationAngle, const bool clockWize);

            bool SetAmplitude(const float amplitude);

            bool SetElevation(const float elevation);

            bool SetCycleDuration(const uint16_t duration);

            float GetDirection(void) const;

            float GetRotation() const;

            bool IsRotated() const;

            float GetAmplitude(void) const;

            float GetElevation(void) const;

            uint16_t GetCycleDuration(void) const;

            float GetCurrentDirection(void) const;

            float GetCurrentRotation() const;

            bool GetCurrentRotationClockWize() const;

            float GetCurrentAmplitude(void) const;

            float GetCurrentElevation(void) const;

        private:
            void UpdateDirection(uint64_t currentTime);

            void UpdateRotation(uint64_t currentTime);

            void UpdateAmplitude(uint64_t currentTime);

            void UpdateElevation(uint64_t currentTime);

            static float GetDeltaTime(float step, uint64_t currentTime, uint64_t startTime);

        private:
            struct LerpPath {
                float current;
                float target;
                uint64_t startTime;
            };

        private:
            LerpPath mLerpDirection;
            LerpPath mLerpAmplitude;
            LerpPath mLerpElevation;
            LerpPath mLerpRotation;
            bool mIsRotated;
            bool mIsRotatedClockWize;
            uint16_t mStepDuration;
        };
    }
}


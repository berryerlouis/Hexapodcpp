#pragma once
#include "../../Misc/Maths/Utils.h"
#include "../../Driver/Tick/TickInterface.h"

namespace Bot
{
    namespace Gait
    {
        class GaitParams {
        public:
            GaitParams(Driver::Tick::TickInterface &tick, const float directionAngle, const float amplitude,
                       const float elevation);

            ~GaitParams() = default;

            void Update(const uint64_t currentTime);

            bool SetDirection(const float directionAngle);

            bool SetAmplitude(const float amplitude);

            bool SetElevation(const float elevation);

            bool SetCycleDuration(const uint16_t duration);

            float GetDirection(void) const;

            float GetAmplitude(void) const;

            float GetElevation(void) const;

            uint16_t GetCycleDuration(void) const;

            float GetCurrentDirection() const;

            float GetCurrentAmplitude() const;

            float GetCurrentElevation() const;

        private:
            void UpdateDirection(uint64_t currentTime);

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
            Driver::Tick::TickInterface &mTick;
            LerpPath mLerpDirection;
            LerpPath mLerpAmplitude;
            LerpPath mLerpElevation;
            uint16_t mStepDuration;
        };
    }
}


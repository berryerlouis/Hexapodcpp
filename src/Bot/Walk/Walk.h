#pragma once
#include "../Constants.h"
#include "../Legs/Legs.h"
#include "../../Misc/Maths/Geometry.h"

namespace Bot
{
    namespace Walk
    {
        using namespace Misc::Maths;
#define NB_STEPS    3U

        class Walk {
        public:
            Walk(Legs::Legs &legs);

            ~Walk() = default;

            void UpdateStatus(const EWalkStatus status);

            void Update(const uint64_t currentTime);

            bool SetDirection(const uint16_t directionAngle);

            bool SetAmplitudeElevation(const uint16_t amplitude, const uint16_t elevation);

            uint16_t GetDirection(void) const;

            uint8_t GetAmplitude(void) const;

            uint8_t GetElevation(void) const;

        private:
            void Play(const uint64_t currentTime);

            void Pause(const uint64_t currentTime);

            void Stop(const uint64_t currentTime);


            struct Step {
                Position3d positions;
                float delayStep;
            };

            struct Steps {
                Step steps[NB_STEPS];
                uint64_t startTime;
                uint8_t idxPosition;
            };

            Legs::Legs &mLegs;
            EWalkStatus mStatus;
            uint8_t mCounterStepPosition;
            uint8_t mMaxCounterStepPosition;
            Steps mWalkStepPosition[NB_LEGS];
            uint16_t mDelayStep;
            uint64_t mPreviousTime;
            bool mStepFinished;
            float mDirection;
            float mAmplitude;
            float mElevation;
        };
    }
}

#pragma once
#include "../Constants.h"
#include "../Legs/Legs.h"
#include "../../Misc/Maths/Geometry.h"
#include "../../Driver/Tick/TickInterface.h"

namespace Bot
{
    namespace WalkCpy
    {
        using namespace Misc::Maths;
#define NB_STEPS    3U

        class WalkCpy {
        public:
            WalkCpy(Legs::Legs &legs, Driver::Tick::TickInterface &tick);

            ~WalkCpy() = default;

            void UpdateStatus(const EWalkStatus status);

            void Update(const uint64_t currentTime);

            /*bool ApplyDirection(const uint16_t directionAngle);

            bool ApplyAmplitude(const uint8_t amplitude);

            bool ApplyElevation(const uint8_t elevation);

            uint16_t GetDirection(void) const;

            uint8_t GetAmplitude(void) const;

            uint8_t GetElevation(void) const;*/

        private:
            void Play(const uint64_t currentTime);

            void Pause(const uint64_t currentTime);

            void Stop(const uint64_t currentTime);

            /*struct Step {
                Position3d positions;
                float delayStep;
                bool onGround;
            };

            struct Steps {
                Step steps[NB_STEPS];
                uint64_t startTime;
                uint8_t idxPosition;
            };


            void UpdateDirection(const uint64_t currentTime);

            void UpdateAmplitude(const uint64_t currentTime);

            void UpdateElevation(const uint64_t currentTime);

            Position3d ComputeNewLegPosition(const float deltaTime, const Steps &legSteps,
                                             const uint8_t legId) const;

            void NextStep(const uint64_t currentTime, Steps &legSteps) const;

            void SetAmplitude(Position3d &position) const;

            void SetElevation(Position3d &position) const;

            static float GetDeltaTime(const float step, const uint64_t currentTime, const uint64_t startTime);

            static Position3d Rotate(const float angle, const Position3d &position, const bool clockWize = true);

*/
            Legs::Legs &mLegs;
            Driver::Tick::TickInterface &mTick;
            EWalkStatus mStatus;
            /*uint8_t mMaxCounterStepPosition;
            Steps mWalkStepPosition[NB_LEGS];
            uint16_t mDelayStep;
            uint64_t mPreviousTime;
            bool mStepFinished;
            uint64_t mStartTimeDirection;
            float mDirection;
            float mTargetDirection;
            uint64_t mStartTimeAmplitude;
            uint64_t mStartTimeElevation;
            float mAmplitude;
            float mTargetAmplitude;
            float mElevation;
            float mTargetElevation;*/

        };
    }
}

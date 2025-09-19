#pragma once

//#include "../../Component/Servo/ServoInterface.h"
#include "LegInterface.h"

namespace Bot
{
    namespace Leg
    {
        using namespace Component::Servo;
        using namespace Misc::Maths;

        class Leg : public LegInterface {
        public:
            static constexpr float BODY_SIDE_X_LENGTH = 80.0;
            static constexpr float BODY_SIDE_X_MIDDLE_LENGTH = 130.0;
            static constexpr float BODY_SIDE_Y_LENGTH = 150.0;

            static constexpr float BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH = BODY_SIDE_X_MIDDLE_LENGTH / 2.0;
            static constexpr float BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH = 0.0;
            static constexpr float BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH = BODY_SIDE_X_LENGTH / 2.0;
            static constexpr float BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH = BODY_SIDE_Y_LENGTH / 2.0;

            static constexpr float COXA_LENGTH = 2.8;
            static constexpr float FEMUR_LENGTH = 8.40;
            static constexpr float TIBIA_LENGTH = 12.70;

            struct SLegIk {
                Position3d newFootPos;
                float coxaFootDist;
                float iksw;
                float ika1;
                float ika2;
                float tangle;
                float coxaIk;
                float femurIk;
                float tibiaIk;
            };


            Leg(const Legs::ELeg legId, ServoInterface &coxa, ServoInterface &femur, ServoInterface &tibia);

            ~Leg() = default;

            void SetTarget(const Position3d &target) final override;

            void ResetTarget(void) final override;

            Core::Status UpdatePosition(const float deltaTime) final override;

            Legs::ELeg GetId(void) const final override;

            Core::Status SetLegIk(const Position3d &position, const uint16_t travelTime = 0U) final override;

            Core::Status SetLegBodyIk(const Position3d &position, const Position3d &bodyIk,
                                      const uint16_t travelTime) final override;

            void ComputeDirection(Position3d &position, float angleDirection) const final override;

            void ComputeRotation(Position3d &position, float angleRotation, bool clockwize) const final override;

            void ComputeAmplitude(Position3d &position, uint8_t amplitude) final override;

            void ComputeElevation(Position3d &position, uint8_t elevation) final override;

            Position3d GetFootPosition(void) const final override;

            float GetBodyCenterOffsetX(void) const final override;

            float GetBodyCenterOffsetY(void) const final override;

        public:
            float mBodyCenterOffsetX;
            float mBodyCenterOffsetY;
            Position3d mFootPosition;
            SLegIk mLegIk;
            Position3d mCurrentPos;
            Position3d mTargetPos;

        private:
            Legs::ELeg mLegId;
            ServoInterface &mCoxa;
            ServoInterface &mFemur;
            ServoInterface &mTibia;
        };
    }
}

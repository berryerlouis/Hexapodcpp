#pragma once

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

            static constexpr float BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH =
                    BODY_SIDE_X_MIDDLE_LENGTH / 2.0;
            static constexpr float BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH = 0.0;
            static constexpr float BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH =
                    BODY_SIDE_X_LENGTH / 2.0;
            static constexpr float BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH =
                    BODY_SIDE_Y_LENGTH / 2.0;

            static constexpr float COXA_LENGTH = 2.8;
            static constexpr float FEMUR_LENGTH = 8.40;
            static constexpr float TIBIA_LENGTH = 12.70;

            // Coxa range shared by every move. Legs are perpendicular to the body and 7.5 cm apart,
            // so neighbouring legs moving in opposite directions touch beyond this swing.
            static constexpr float COXA_ANGLE_MIN = 84.0F;
            static constexpr float COXA_ANGLE_MAX = 96.0F;

            // Servo angles in degrees, neutral pose is 90/90/90.
            struct SLegAngles {
                float coxa;
                float femur;
                float tibia;
            };

            // Foot position relative to the coxa axis, in the leg frame (x radial,
            // y tangent, z down). Targets outside the workspace are projected onto it.
            // Returns false only for non-finite input.
            static bool SolveIk(const Position3d &foot, SLegAngles &angles);

            Leg(const Legs::ELeg legId,
                ServoInterface  &coxa,
                ServoInterface  &femur,
                ServoInterface  &tibia);

            ~Leg() = default;

            void         SetTarget(const Position3d &target) final override;

            Core::Status Update(void) final override;

            Position3d  &GetCurrentPosition(void) final override;

            void         ComputeTarget(Misc::Maths::Position3d &position,
                                       const float              amplitude,
                                       const float              elevation,
                                       const float              direction,
                                       const bool               isRotated,
                                       const float              rotation,
                                       const bool               clockWize) const final override;

            Legs::ELeg   GetId(void) const final override;

            Core::Status SetLegIk(const Position3d &position,
                                  const uint16_t    travelTime) final override;

            Core::Status SetLegBodyIk(const Position3d &position,
                                      const Position3d &bodyIk,
                                      const uint16_t    travelTime) final override;

            void ComputeDirection(Position3d &position, float angleDirection) const final override;

            void ComputeRotation(Position3d &position,
                                 float       angleRotation,
                                 bool        clockwize) const final override;

            void ComputeAmplitude(Position3d &position, float amplitude) const final override;

            void ComputeElevation(Position3d &position, float elevation) const final override;

            Position3d GetFootPosition(void) const final override;

            float      GetBodyCenterOffsetX(void) const final override;

            float      GetBodyCenterOffsetY(void) const final override;

        private:
            Core::Status ApplyAngles(const SLegAngles &angles, const uint16_t travelTime);

            // Offsets of the coxa axis from the body center, in cm like the leg lengths.
            float           mBodyCenterOffsetX;
            float           mBodyCenterOffsetY;
            // Direction of the coxa axis in the body frame, atan2(y, x) in radians.
            float           mMountAngle;
            Position3d      mFootPosition;
            Position3d      mCurrentPos;
            Legs::ELeg      mLegId;
            ServoInterface &mCoxa;
            ServoInterface &mFemur;
            ServoInterface &mTibia;
        };
    } // namespace Leg
} // namespace Bot

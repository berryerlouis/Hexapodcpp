#pragma once

#include "../../Component/Servo/ServoInterface.h"
#include "../../Misc/Maths/Geometry.h"
#include "../Constants.h"
#include "../../Move/Gait/GaitParams.h"

namespace Bot
{
    namespace Leg
    {
        using namespace Misc::Maths;

        class LegInterface {
        public:
            virtual ~LegInterface() = default;

            virtual void SetTarget(const Position3d &target) = 0;

            virtual void ResetTarget(void) = 0;

            virtual Core::Status Update(void) = 0;

            virtual void ComputeLerpTarget(const uint64_t currentTime, Misc::Maths::Position3d &position, const Move::Gait::GaitParams &gaitParams) = 0;

            virtual Legs::ELeg GetId(void) const = 0;

            virtual Core::Status SetLegIk(const Position3d &position, const uint16_t travelTime = 0U) = 0;

            virtual Core::Status SetLegBodyIk(const Position3d &position, const Position3d &bodyIk,
                                              const uint16_t travelTime) = 0;

            virtual void ComputeDirection(Position3d &position, float angleDirection) const = 0;

            virtual void ComputeRotation(Position3d &position, float angleRotation, bool clockwize) const = 0;

            virtual void ComputeAmplitude(Position3d &position, uint8_t amplitude) = 0;

            virtual void ComputeElevation(Position3d &position, uint8_t elevation) = 0;

            virtual Position3d GetFootPosition() const = 0;

            virtual float GetBodyCenterOffsetX(void) const = 0;

            virtual float GetBodyCenterOffsetY(void) const = 0;
        };
    }
}
#pragma once

#include "../../Move/Gait/Constants.h"
#include "../../Move/Walk/Constants.h"
#include "../Legs/Legs.h"
#include "../../Component/ComponentInterface.h"
#include "../../Misc/Maths/Geometry.h"

namespace Bot
{
    namespace Body
    {
        using namespace Misc::Maths;

        class BodyInterface : public Component::ComponentInterface<0U, nullptr_t> {
        public:
            BodyInterface() = default;

            ~BodyInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual void UpdateWalkStatus(const Move::Walk::EWalkStatus status, const uint16_t duration) = 0;

            virtual Move::Walk::EWalkStatus GetWalkStatus(void) = 0;

            virtual bool SetDirection(const float directionAngle) = 0;

            virtual bool SetRotation(const float rotationAngle, const bool clockWize) = 0;

            virtual bool SetAmplitude(const float amplitude) = 0;

            virtual bool SetElevation(const float elevation) = 0;

            virtual bool SetDuration(const uint16_t duration) = 0;

            virtual bool SetGait(const Move::Gait::GaitType gait) = 0;

            virtual float GetDirection(void) = 0;

            virtual float GetRotation(void) = 0;

            virtual bool GetRotationClockWize(void) = 0;

            virtual float GetAmplitude(void) = 0;

            virtual float GetElevation(void) = 0;

            virtual uint16_t GetDuration(void) = 0;

            virtual Move::Gait::GaitType GetGait(void) = 0;

            virtual uint32_t SetBodyPositionRotation(const Position3d &position,
                                                     const Rotation3d &rotation,
                                                     const uint16_t travelTime) = 0;

            virtual uint32_t SetLegPositionRotation(const uint8_t &legId,
                                                    const Position3d &position,
                                                    const uint16_t travelTime) = 0;
        };
    }
}

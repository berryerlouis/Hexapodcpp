#pragma once

#include "../Constants.h"
#include "../Legs/Legs.h"
#include "../../Component/ComponentInterface.h"
#include "../../Misc/Maths/Geometry.h"

namespace Bot
{
    namespace Body
    {
        using namespace Misc::Maths;

        class BodyInterface : public Component::ComponentInterface {
        public:
            BodyInterface() = default;

            ~BodyInterface() = default;

            virtual Core::CoreStatus Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual void UpdateWalkStatus(const EWalkStatus status) = 0;

            virtual uint32_t SetBodyPositionRotation(const Position3d &position,
                                                     const Rotation3d &rotation,
                                                     const uint16_t travelTime) = 0;

            virtual uint32_t SetLegPositionRotation(const uint8_t &legId,
                                                    const Position3d &position,
                                                    const uint16_t travelTime) = 0;
        };
    }
}

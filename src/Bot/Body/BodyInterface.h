#pragma once

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

            virtual void SetPositionRotation(const Misc::Maths::Position3d &position,
                                             const Misc::Maths::Rotation3d &rotation, const uint16_t travelTime) = 0;
        };
    }
}

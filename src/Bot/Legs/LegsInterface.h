#pragma once

#include "../Leg/Leg.h"
#include "../../Move/Gait/GaitParams.h"
#include <vector>

namespace Bot
{
    namespace Legs
    {
        class LegsInterface {
        public:
            virtual ~LegsInterface() = default;

            virtual Leg::LegInterface *GetLeg(const ELeg legId) = 0;
            virtual void ResetLegs(const uint16_t cycleDuration) = 0;
            virtual Core::Status Update(void) = 0;
            virtual void SetTarget(const Move::Gait::GaitParams &gaitParams,
                                const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                                const uint8_t stepPositionIndex, 
                                const float normalizedTime) = 0;
            
        };
    }
}

#pragma once

#include <map>
#include <vector>
#include "../../Move/Gait/GaitParams.h"
#include "../Leg/Leg.h"

namespace Bot
{
    namespace Legs
    {
        class LegsInterface {
        public:
            virtual ~LegsInterface() = default;

            virtual std::map<ELeg, Leg::Leg> &GetLegs(void) = 0;

            virtual Leg::LegInterface        *GetLeg(const ELeg legId) = 0;

            virtual Core::Status              Update(void) = 0;
        };
    } // namespace Legs
} // namespace Bot

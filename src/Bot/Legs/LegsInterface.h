#pragma once

#include "../Leg/Leg.h"

namespace Bot
{
    namespace Legs
    {
        class LegsInterface {
        public:
            virtual ~LegsInterface() = default;

            virtual Leg::LegInterface *GetLeg(const ELeg legId) = 0;
        };
    }
}

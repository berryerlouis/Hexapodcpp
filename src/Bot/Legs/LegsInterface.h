#pragma once

#include "../Leg/LegInterface.h"

namespace Bot
{
    namespace Legs
    {
        class LegsInterface {
        public:
            virtual ~LegsInterface() = default;

            virtual Leg::LegInterface *GetLeg(const ELeg legId) const = 0;
        };
    }
}

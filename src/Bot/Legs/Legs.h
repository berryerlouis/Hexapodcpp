#pragma once

#include "../../Component/Servos/ServosInterface.h"
#include "../Leg/Leg.h"
#include "../Gait/GaitParams.h"
#include "../Constants.h"

namespace Bot
{
    namespace Legs
    {
        using namespace Component::Servos;

        class Legs {
        public:
            Legs(ServosInterface &servos);

            ~Legs() = default;

            Leg::Leg *GetLeg(const uint8_t legId) const;

            const Leg::Leg *GetLegs(void) const;

        private:
            Leg::Leg mLegFL;
            Leg::Leg mLegML;
            Leg::Leg mLegRL;
            Leg::Leg mLegFR;
            Leg::Leg mLegMR;
            Leg::Leg mLegRR;
            Leg::Leg *mLegs[NB_LEGS];
        };
    }
}

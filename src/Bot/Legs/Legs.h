#pragma once

#include "LegsInterface.h"
#include "../Leg/Leg.h"
#include "../../Component/Servos/ServosInterface.h"
#include "../Constants.h"

namespace Bot
{
    namespace Legs
    {
        using namespace Component::Servos;

        class Legs : public LegsInterface {
        public:
            Legs(ServosInterface &servos);

            ~Legs() = default;

            Leg::LegInterface *GetLeg(const ELeg legId) const final override;

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

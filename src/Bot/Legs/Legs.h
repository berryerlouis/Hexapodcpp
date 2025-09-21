#pragma once

#include <unordered_map>

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

            Leg::LegInterface *GetLeg(const ELeg legId) final override;

        private:
            std::unordered_map<ELeg, Leg::Leg> mLegs;
        };
    }
}

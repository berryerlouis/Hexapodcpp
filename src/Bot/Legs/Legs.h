#pragma once

#include "../../Component/Servos/ServosInterface.h"
#include "../Constants.h"
#include "../Leg/Leg.h"
#include "LegsInterface.h"

namespace Bot
{
    namespace Legs
    {
        using namespace Component::Servos;

        class Legs : public LegsInterface {
        public:
            explicit Legs(ServosInterface &servos);

            ~Legs() = default;

            std::map<ELeg, Leg::Leg> &GetLegs(void) final override;

            Leg::LegInterface        *GetLeg(const ELeg legId) final override;

            Core::Status              Update(void) final override;

        private:
            std::map<ELeg, Leg::Leg> mLegs;
            uint8_t                  mLegIdInWaveGait = 0U;
        };
    } // namespace Legs
} // namespace Bot

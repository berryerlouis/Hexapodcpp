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

            void ResetLegs(const uint16_t cycleDuration) final override;

            Core::Status Update(void) final override;

            void ComputeTarget(const uint64_t currentTime,
                           const Move::Gait::GaitParams &gaitParams,
                           const std::vector<std::vector<Misc::Maths::Position3d> > &positions,
                           const uint8_t stepPositionIndex,
                           const float normalizedTime) final override;

        private:

            uint8_t GetStepIndexPosition(const uint8_t legId,
                                         const uint8_t nbPositions,
                                         const uint8_t stepPositionIndex, 
                                         const Move::Gait::GaitType gaitType);

            std::unordered_map<ELeg, Leg::Leg> mLegs;
            uint8_t mLegIdInWaveGait = 0U;
        };
    }
}
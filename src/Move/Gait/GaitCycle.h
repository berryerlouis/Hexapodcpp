#pragma once
#include <vector>

#include "../../Bot/Legs/LegsInterface.h"
#include "GaitParams.h"
#include "Gaits.h"
#include "../../Driver/Tick/TickInterface.h"

namespace Move
{
    namespace Gait
    {
        class GaitCycle {
        public:
            GaitCycle(Bot::Legs::LegsInterface &legs, const GaitType gait,
                      Driver::Tick::TickInterface &tick);

            GaitBase &GetSelectedGait() const;

            bool SetGaitType(GaitType gaitType);

            GaitType GetGaitType() const;

            GaitParams &GetGaitParams();

            ~GaitCycle() = default;

            void Update(const uint64_t currentTime);

            void ResetCycleStep() const;

            bool Start(void);

            bool Pause(void);

            bool Stop(void);

        private:
            void SetLegTarget(const Bot::Legs::ELeg legId, const uint8_t posId, const float deltaTime) const;

            void UpdateCycle(const uint64_t currentTime) const;

            Bot::Legs::LegsInterface &mLegs;
            GaitParams mParams;
            Driver::Tick::TickInterface &mTick;
            bool mIsRunning;
            Gaits mGaits;
        };
    }
}

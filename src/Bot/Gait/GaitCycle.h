#pragma once
#include <vector>

#include "../Constants.h"
#include "../Legs/Legs.h"
#include "GaitParams.h"
#include "GaitTripod.h"
#include "Gaits.h"
#include "../../Misc/Maths/Geometry.h"
#include "../../Driver/Tick/TickInterface.h"

namespace Bot
{
    namespace Gait
    {
        class GaitCycle {
        public:
            GaitCycle(Legs::Legs &legs, GaitParams &params, const GaitType gait, Driver::Tick::TickInterface &tick);

            GaitBase &GetSelectedGait() const;

            bool SetGaitType(GaitType gaitType);

            GaitType GetGaitType() const;

            ~GaitCycle() = default;

            void Update(const uint64_t currentTime) const;

            bool Start(void);

            bool Pause(void);

            bool Stop(void);

        private:
            void SetLegTarget(const uint8_t legId, const uint8_t posId, const float deltaTime) const;

            void UpdateCycle(const uint64_t currentTime) const;

            Legs::Legs &mLegs;
            GaitParams &mParams;
            Driver::Tick::TickInterface &mTick;
            bool mIsRunning;
            Gaits mGaits;
        };
    }
}

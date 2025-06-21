#pragma once

#include "GaitParams.h"
#include "GaitTripod.h"
#include "GaitWave.h"
#include "Constants.h"
#include "GaitDoubleWave.h"

namespace Bot
{
    namespace Gait
    {
        class Gaits {

        public:
            Gaits(GaitParams &params, const GaitType gait);

            ~Gaits() = default;

            bool SetGaitType(const GaitType gaitType);

            GaitType GetGaitType() const;

            GaitBase &GetSelectedGait() const;

        private:
            GaitParams &mParams;
            GaitTripod mTripod;
            GaitWave mWave;;
            GaitDoubleWave mDoubleWave;
            GaitBase *mGaitTypes[NB_GAIT_TYPES];
            GaitType mGaitTypeSelected;
        };
    }
}

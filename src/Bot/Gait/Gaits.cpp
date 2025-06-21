#include "Gaits.h"
#include "../Constants.h"

namespace Bot
{
    namespace Gait
    {
        Gaits::Gaits(GaitParams &params, const GaitType gait):
            mParams(params)
            , mTripod(GaitTripod(params))
            , mWave(GaitWave(params))
            , mDoubleWave(GaitDoubleWave(params))
            , mGaitTypes{&mTripod, &mWave, &mDoubleWave}
            , mGaitTypeSelected(gait) {
            this->SetGaitType(gait);
        }

        bool Gaits::SetGaitType(const GaitType gaitType) {
            LOG_BOT_DEBUG("Gaits", "SetGaitType: %s", GaitTypeToString(gaitType).c_str());
            this->mGaitTypeSelected = gaitType;
            return true;
        }

        GaitType Gaits::GetGaitType(void) const {
            return this->mGaitTypeSelected;
        }

        GaitBase &Gaits::GetSelectedGait(void) const {
            return *this->mGaitTypes[this->mGaitTypeSelected];
        }
    }

}



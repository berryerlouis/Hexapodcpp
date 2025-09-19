#include "Gaits.h"

namespace Move
{
    namespace Gait
    {
        Gaits::Gaits(GaitParams &params, const GaitType gait):
            mParams(params)
            , mTripod(GaitTripod(params))
            , mWave(GaitWave(params))
            , mRipple(GaitRipple(params))
            , mDoubleWave(GaitDoubleWave(params))
            , mGaitTypes{&mTripod, &mWave, &mRipple, &mDoubleWave}
            , mGaitTypeSelected(gait) {
            this->SetGaitType(gait);
        }

        bool Gaits::SetGaitType(const GaitType gaitType) {
            LOG_MOVE_DEBUG("Gaits", "SetGaitType: %s", GaitTypeToString(gaitType).c_str());
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



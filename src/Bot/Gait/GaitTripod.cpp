#include "GaitTripod.h"

namespace Bot
{
    namespace Gait
    {
        GaitTripod::GaitTripod(GaitParams &params) :
            GaitBase(TRIPOD, {&mGroupA, &mGroupB}, params)
            , mGroupA{{FRONT_LEFT, MIDDLE_RIGHT, REAR_LEFT}, 0.0F / 2.0F}
            , mGroupB{{FRONT_RIGHT, MIDDLE_LEFT, REAR_RIGHT}, 1.0F / 2.0F} {

            this->GetSteps().steps[0U].duration = 1.0F / 4.0F;
            this->GetSteps().steps[1U].duration = 1.0F / 4.0F;
            this->GetSteps().steps[2U].duration = 2.0F / 4.0F;
        }
    }
}

#include "GaitTripod.h"

namespace Bot
{
    namespace Gait
    {
        GaitTripod::GaitTripod(GaitParams &params) :
            GaitBase(TRIPOD, {&mGroupA, &mGroupB}, params)
            , mGroupA{{FRONT_LEFT, MIDDLE_RIGHT, REAR_LEFT}, 0.0F / 2.0F}
            , mGroupB{{FRONT_RIGHT, MIDDLE_LEFT, REAR_RIGHT}, 1.0F / 2.0F} {

            this->GetSteps().SetStepDuration(0U, 1.0F / 4.0F);
            this->GetSteps().SetStepDuration(1U, 1.0F / 4.0F);
            this->GetSteps().SetStepDuration(2U, 2.0F / 4.0F);
        }
    }
}

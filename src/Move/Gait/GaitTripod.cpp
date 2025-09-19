#include "GaitTripod.h"

namespace Move
{
    namespace Gait
    {
        GaitTripod::GaitTripod(GaitParams &params) :
            GaitBase(TRIPOD, {&mGroupA, &mGroupB}, params)
            , mGroupA{{Bot::Legs::FRONT_LEFT, Bot::Legs::MIDDLE_RIGHT, Bot::Legs::REAR_LEFT}, 0.0F / 2.0F}
            , mGroupB{{Bot::Legs::FRONT_RIGHT, Bot::Legs::MIDDLE_LEFT, Bot::Legs::REAR_RIGHT}, 1.0F / 2.0F} {

            this->GetSteps().SetStepDuration(0U, 1.0F / 4.0F);
            this->GetSteps().SetStepDuration(1U, 1.0F / 4.0F);
            this->GetSteps().SetStepDuration(2U, 2.0F / 4.0F);
        }
    }
}

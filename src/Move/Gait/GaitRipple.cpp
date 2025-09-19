#include "GaitRipple.h"

namespace Move
{
    namespace Gait
    {
        GaitRipple::GaitRipple(GaitParams &params) :
            GaitBase(TRIPOD, {&mGroupA, &mGroupB, &mGroupC, &mGroupD}, params)
            , mGroupA{{Bot::Legs::FRONT_LEFT, Bot::Legs::REAR_RIGHT}, 0.0F / 4.0F}
            , mGroupB{{Bot::Legs::MIDDLE_LEFT}, 1.0F / 4.0F}
            , mGroupC{{Bot::Legs::REAR_LEFT, Bot::Legs::FRONT_RIGHT}, 2.0F / 4.0F}
            , mGroupD{{Bot::Legs::MIDDLE_RIGHT}, 3.0F / 4.0F} {

            this->GetSteps().SetStepDuration(0U, 1.0F / 8.0F);
            this->GetSteps().SetStepDuration(1U, 1.0F / 8.0F);
            this->GetSteps().SetStepDuration(2U, 6.0F / 8.0F);
        }
    }
}

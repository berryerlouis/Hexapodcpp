#include "GaitRipple.h"

namespace Bot
{
    namespace Gait
    {
        GaitRipple::GaitRipple(GaitParams &params) :
            GaitBase(TRIPOD, {&mGroupA, &mGroupB, &mGroupC, &mGroupD}, params)
            , mGroupA{{FRONT_LEFT, REAR_RIGHT}, 0.0F / 4.0F}
            , mGroupB{{MIDDLE_LEFT}, 1.0F / 4.0F}
            , mGroupC{{REAR_LEFT, FRONT_RIGHT}, 2.0F / 4.0F}
            , mGroupD{{MIDDLE_RIGHT}, 3.0F / 4.0F} {

            this->GetSteps().SetStepDuration(0U, 1.0F / 8.0F);
            this->GetSteps().SetStepDuration(1U, 1.0F / 8.0F);
            this->GetSteps().SetStepDuration(2U, 6.0F / 8.0F);
        }
    }

}

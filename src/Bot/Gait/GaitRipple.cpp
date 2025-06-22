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

            this->GetSteps().steps[0U].duration = 1.0F / 8.0F;
            this->GetSteps().steps[1U].duration = 1.0F / 8.0F;
            this->GetSteps().steps[2U].duration = 6.0F / 8.0F;
        }
    }

}

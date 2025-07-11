#include "GaitWave.h"

namespace Bot
{
    namespace Gait
    {

        GaitWave::GaitWave(GaitParams &params) :
            GaitBase(WAVE, {&mGroupA, &mGroupB, &mGroupC, &mGroupD, &mGroupE, &mGroupE, &mGroupF}, params),
            mGroupA{{FRONT_LEFT}, 0.0F / 6.0F},
            mGroupB{{MIDDLE_LEFT}, 1.0F / 6.0F},
            mGroupC{{REAR_LEFT}, 2.0F / 6.0F},
            mGroupD{{REAR_RIGHT}, 3.0F / 6.0F},
            mGroupE{{MIDDLE_RIGHT}, 4.0F / 6.0F},
            mGroupF{{FRONT_RIGHT}, 5.0F / 6.0F} {

            this->GetSteps().SetStepDuration(0U, 1.0F / 12.0F);
            this->GetSteps().SetStepDuration(1U, 1.0F / 12.0F);
            this->GetSteps().SetStepDuration(2U, 10.0F / 12.0F);
        }
    }
}

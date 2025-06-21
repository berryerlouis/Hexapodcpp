#include "GaitWave.h"

namespace Bot
{
    namespace Gait
    {

        GaitWave::GaitWave(const GaitParams &params) :
            GaitBase(WAVE, {&mGroupA, &mGroupB, &mGroupC, &mGroupD, &mGroupE, &mGroupE, &mGroupF}, params),
            mGroupA{{FRONT_LEFT}, 0.0F / 6.0F},
            mGroupB{{MIDDLE_LEFT}, 1.0F / 6.0F},
            mGroupC{{REAR_LEFT}, 2.0F / 6.0F},
            mGroupD{{REAR_RIGHT}, 3.0F / 6.0F},
            mGroupE{{MIDDLE_RIGHT}, 4.0F / 6.0F},
            mGroupF{{FRONT_RIGHT}, 5.0F / 6.0F} {

            this->GetSteps().steps[0U].duration = 1.0F / 12.0F;
            this->GetSteps().steps[1U].duration = 1.0F / 12.0F;
            this->GetSteps().steps[2U].duration = 10.0F / 12.0F;
        }
    }
}

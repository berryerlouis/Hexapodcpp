#include "GaitDoubleWave.h"

namespace Bot
{
    namespace Gait
    {

        GaitDoubleWave::GaitDoubleWave(GaitParams &params) :
            GaitBase(WAVE, {&mGroupA, &mGroupB, &mGroupC}, params),
            mGroupA{{FRONT_LEFT, FRONT_RIGHT}, 0.0F / 3.0F},
            mGroupB{{MIDDLE_LEFT, MIDDLE_RIGHT}, 1.0F / 3.0F},
            mGroupC{{REAR_LEFT, REAR_RIGHT}, 2.0F / 3.0F} {

            this->GetSteps().steps[0U].duration = 1.0F / 6.0F;
            this->GetSteps().steps[1U].duration = 1.0F / 6.0F;
            this->GetSteps().steps[2U].duration = 4.0F / 6.0F;
        }
    }
}

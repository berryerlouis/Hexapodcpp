#include "GaitDoubleWave.h"

namespace Move
{
    namespace Gait
    {
        GaitDoubleWave::GaitDoubleWave(GaitParams &params) :
            GaitBase(WAVE, {&mGroupA, &mGroupB, &mGroupC}, params),
            mGroupA{{Bot::Legs::FRONT_LEFT, Bot::Legs::REAR_RIGHT}, 0.0F / 3.0F},
            mGroupB{{Bot::Legs::MIDDLE_LEFT, Bot::Legs::MIDDLE_RIGHT}, 1.0F / 3.0F},
            mGroupC{{Bot::Legs::REAR_LEFT, Bot::Legs::FRONT_RIGHT}, 2.0F / 3.0F} {

            this->GetSteps().steps[0U].duration = 1.0F / 6.0F;
            this->GetSteps().steps[1U].duration = 1.0F / 6.0F;
            this->GetSteps().steps[2U].duration = 4.0F / 6.0F;
        }
    }
}

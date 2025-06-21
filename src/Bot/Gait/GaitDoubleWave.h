#pragma once

#include "GaitBase.h"

namespace Bot
{
    namespace Gait
    {
        class GaitDoubleWave : public GaitBase {
        public:
            GaitDoubleWave(const GaitParams &params);

            ~GaitDoubleWave() = default;

        private:
            Group mGroupA;
            Group mGroupB;
            Group mGroupC;
            Groups mGroups;
        };
    }
}

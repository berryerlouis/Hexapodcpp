#pragma once

#include "GaitBase.h"

namespace Bot
{
    namespace Gait
    {
        class GaitWave : public GaitBase {
        public:
            GaitWave(GaitParams &params);

            ~GaitWave() = default;

        private:
            Group mGroupA;
            Group mGroupB;
            Group mGroupC;
            Group mGroupD;
            Group mGroupE;
            Group mGroupF;
            Groups mGroups;
        };
    }
}

#pragma once

#include "GaitBase.h"

namespace Move
{
    namespace Gait
    {
        class GaitRipple : public GaitBase {
        public:
            GaitRipple(GaitParams &params);

            ~GaitRipple() = default;

        private:
            Group mGroupA;
            Group mGroupB;
            Group mGroupC;
            Group mGroupD;
        };
    }
}

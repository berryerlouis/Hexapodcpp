#pragma once

#include "GaitBase.h"

namespace Bot
{
    namespace Gait
    {
        class GaitTripod : public GaitBase {
        public:
            GaitTripod(GaitParams &params);

            ~GaitTripod() = default;

        private:
            Group mGroupA;
            Group mGroupB;
        };
    }
}

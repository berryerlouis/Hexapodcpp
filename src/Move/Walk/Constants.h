#pragma once

#include <string>
#include "../Constants.h"

namespace Move
{

    namespace Walk
    {
        enum EWalkStatus {
            PLAY = 0x00U,
            PAUSE,
            STOP,
        };

        static inline std::string WalkStatusToString(const EWalkStatus status) {
            switch (status) {
                case PLAY:
                    return "PLAY";
                case PAUSE:
                    return "PAUSE";
                case STOP:
                    return "STOP";
                default:
                    return "UNKNOWN";
            }
        }
    }
}

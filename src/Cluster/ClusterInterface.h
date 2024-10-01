#pragma once

#include "Constants.h"
#include "Frame/Frame.h"
#include "../Misc/Logger/Logger.h"

namespace Cluster
{
    class ClusterInterface {
    public:
        ClusterInterface() = default;

        ~ClusterInterface() = default;

        virtual Core::CoreStatus Execute(Frame &request, Frame &response) = 0;

        virtual EClusters GetId(void) = 0;

        virtual Core::CoreStatus BuildFrameNack(Frame &response) = 0;

        virtual Core::CoreStatus BuildFrameNack(Frame &response, const Core::CoreStatus error) = 0;
    };
}

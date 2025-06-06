#pragma once

#include "Constants.h"
#include "Frame/Frame.h"

namespace Cluster
{
    class ClusterInterface {
    public:
        ClusterInterface() = default;

        ~ClusterInterface() = default;

        virtual Core::Status Execute(Frame &request, Frame &response) = 0;

        virtual EClusters GetClusterId(void) = 0;

        virtual Core::Status BuildFrameNack(Frame &response) = 0;

        virtual Core::Status BuildFrameNack(Frame &response, const Core::Status error) = 0;
    };
}

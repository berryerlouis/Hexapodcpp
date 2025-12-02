#pragma once

#include "Constants.h"
#include "Frame/Frame.h"

namespace Cluster
{
#define LOG_CLUSTER_DEBUG(cmp, ...) LOG_GENERIC(DEBUG, CLUSTER, "[" cmp "] " __VA_ARGS__)
#define LOG_CLUSTER_INFO(cmp, ...)  LOG_GENERIC(INFO, CLUSTER,  "[" cmp "] "  __VA_ARGS__)
#define LOG_CLUSTER_WARNING(cmp, ...) LOG_GENERIC(WARNING, CLUSTER, "[" cmp "] "  __VA_ARGS__)
#define LOG_CLUSTER_ERROR(cmp, ...) LOG_GENERIC(ERROR, CLUSTER, "[" cmp "] "  __VA_ARGS__)

    class ClusterInterface {
    public:
        ClusterInterface() = default;

        virtual ~ClusterInterface() = default;

        virtual Core::Status Execute(Frame &request, Frame &response) = 0;

        virtual EClusters GetClusterId() const = 0;

        virtual Core::Status BuildFrameNack(Frame &response) = 0;

        virtual Core::Status BuildFrameNack(Frame &response, Core::Status error) = 0;
    };
}
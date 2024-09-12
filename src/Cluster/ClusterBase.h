#pragma once

#include "ClusterInterface.h"
#include "../Misc/Logger/Logger.h"

namespace Cluster
{
    class ClusterBase : public ClusterInterface {
    public:
        ClusterBase(EClusters clusterId)
            : mClusterId(clusterId) {
        }

        virtual Core::CoreStatus Execute(Frame &request, Frame &response) = 0;

        virtual EClusters GetId(void) {
            return (mClusterId);
        }

    protected:
        const EClusters mClusterId;
    };
}

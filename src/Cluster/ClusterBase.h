#pragma once

#include "ClusterCommand.h"
#include "ClusterInterface.h"

namespace Cluster
{
    class ClusterBase : public ClusterInterface {
    public:
        ClusterBase(const EClusters clusterId, ClusterCommand &strategyCluster);

        Core::Status Execute(Frame &request, Frame &response) final override;

        EClusters    GetClusterId() const final override;

    protected:
        const EClusters mClusterId;
        ClusterCommand &mClusterCommand;
    };
} // namespace Cluster

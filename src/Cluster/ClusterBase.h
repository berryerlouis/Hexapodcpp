#pragma once

#include "ClusterCommand.h"
#include "ClusterInterface.h"

namespace Cluster
{
    class ClusterBase : public ClusterInterface {
    public:
        ClusterBase(const EClusters clusterId,
                    ClusterCommand &strategyCluster);

        Core::Status Execute(Frame &request,
                             Frame &response) final override;

        EClusters    GetClusterId() const final override;

        Core::Status BuildFrameNack(Frame &response) final override;

        Core::Status BuildFrameNack(Frame             &response,
                                    const Core::Status error) final override;

    protected:
        const EClusters mClusterId;
        ClusterCommand &mClusterCommand;
    };
} // namespace Cluster

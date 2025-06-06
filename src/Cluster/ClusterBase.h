#pragma once

#include "ClusterInterface.h"
#include "ClusterCommand.h"

namespace Cluster
{
    class ClusterBase : public ClusterInterface {
    public:
        ClusterBase(const EClusters clusterId, ClusterCommand *strategyCluster);

        virtual Core::Status Execute(Frame &request, Frame &response) final override;

        virtual EClusters GetClusterId(void);

        virtual Core::Status BuildFrameNack(Frame &response);

        virtual Core::Status BuildFrameNack(Frame &response, const Core::Status error);

    protected:
        const EClusters mClusterId;
        ClusterCommand *mClusterCommand;
    };
}

#include "ClusterBase.h"

namespace Cluster
{
    ClusterBase::ClusterBase(const EClusters clusterId,
                             ClusterCommand &strategyCluster) :
        mClusterId(clusterId),
        mClusterCommand(strategyCluster) {
    }

    Core::Status ClusterBase::Execute(Frame &request,
                                      Frame &response) {
        if (request.GetClusterId() != this->GetClusterId()) {
            return Core::Status::CORE_ERROR;
        }
        return this->mClusterCommand.Execute(request, response);
    }

    EClusters ClusterBase::GetClusterId() const {
        return this->mClusterId;
    }

    Core::Status ClusterBase::BuildFrameNack(Frame &response) {
        const Core::Status success = response.Build(this->mClusterId, GENERIC);
        response.Set1ByteParam(false);
        return success;
    }

    Core::Status ClusterBase::BuildFrameNack(Frame             &response,
                                             const Core::Status error) {
        const Core::Status success = response.Build(this->mClusterId, GENERIC);
        response.Set1ByteParam(error);
        return success;
    }
} // namespace Cluster

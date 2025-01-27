#include "ClusterBase.h"

namespace Cluster
{
    ClusterBase::ClusterBase(const EClusters clusterId, ClusterCommand *strategyCluster) :
        mClusterId(clusterId)
        , mClusterCommand(strategyCluster) {
    }

    Core::Status ClusterBase::Execute(Frame &request, Frame &response) {
        Core::Status success = Core::Status::CORE_ERROR;
        if (this->mClusterCommand != nullptr) {
            if (request.clusterId != this->GetId()) {
                return (success);
            }
            success = this->mClusterCommand->Execute(request, response);
        }
        return (success);
    }

    EClusters ClusterBase::GetId(void) {
        return (this->mClusterId);
    }

    Core::Status ClusterBase::BuildFrameNack(Frame &response) {
        response.clusterId = this->mClusterId;
        response.commandId = static_cast<uint8_t>(GENERIC);
        response.Set1ByteParam(false);
        return (Core::Status::CORE_OK);
    }

    Core::Status ClusterBase::BuildFrameNack(Frame &response, const Core::Status error) {
        response.clusterId = this->mClusterId;
        response.commandId = static_cast<uint8_t>(GENERIC);
        response.Set1ByteParam(error);
        return (Core::Status::CORE_OK);
    }
}

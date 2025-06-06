#include "ClusterCommand.h"

namespace Cluster
{

    ClusterCommand::ClusterCommand(const uint8_t size) :
        mClusterCommands{}
        , mClusterCommandSize(0U)
        , mSize(size) {
    }

    bool ClusterCommand::AddClusterItem(const ClusterItem clusterItem) {
        if (this->mClusterCommandSize < this->mSize) {
            mClusterCommands[this->mClusterCommandSize++] = clusterItem;
            return true;
        }
        return false;
    }

    Core::Status ClusterCommand::Execute(const Frame &request, Frame &response) {
        Core::Status success = Core::Status::CORE_ERROR;
        const uint8_t cmdId = request.GetCommandId();
        if (cmdId < this->mClusterCommandSize) {
            if (this->mClusterCommands[cmdId].expectedSize == request.GetNbParams()) {
                success = this->ExecuteFrame(request, response);
            }
        }
        return success;
    }
}

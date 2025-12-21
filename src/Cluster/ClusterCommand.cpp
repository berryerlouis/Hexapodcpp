#include "ClusterCommand.h"

namespace Cluster
{
    ClusterCommand::ClusterCommand(const uint8_t size) :
        mClusterCommands{},
        mClusterCommandSize(0U),
        mSize(size < NB_COMMANDS_MAX ? size : NB_COMMANDS_MAX) {
    }

    bool ClusterCommand::AddClusterItem(const ClusterItem clusterItem) {
        if (this->mClusterCommandSize >= this->mSize) {
            return false;
        }
        if (this->mClusterCommandSize >= NB_COMMANDS_MAX) {
            return false;
        }
        mClusterCommands[this->mClusterCommandSize++] = clusterItem;
        return true;
    }

    Core::Status ClusterCommand::Execute(const Frame &request,
                                         Frame       &response) {
        const uint8_t cmdId = request.GetCommandId();

        // Bounds check
        if (cmdId >= this->mClusterCommandSize) {
            return Core::Status::CORE_ERROR;
        }

        // Validate expected parameter size
        if (this->mClusterCommands[cmdId].expectedSize != request.GetNbParams()) {
            return Core::Status::CORE_ERROR;
        }

        return this->ExecuteFrame(request, response);
    }
} // namespace Cluster

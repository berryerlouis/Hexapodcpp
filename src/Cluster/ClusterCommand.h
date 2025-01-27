#pragma once

#include "Frame/Frame.h"
#include "ClusterItem.h"

namespace Cluster
{
    class ClusterCommand {

    public:
        ClusterCommand(const uint8_t size);

        ~ClusterCommand() = default;

        bool AddClusterItem(const ClusterItem clusterItem);

        virtual Core::Status ExecuteFrame(const Frame &request, Frame &response) = 0;

        Core::Status Execute(const Frame &request, Frame &response);

    private:
        static constexpr uint8_t NB_COMMANDS_MAX = 20U;
        ClusterItem mClusterCommands[NB_COMMANDS_MAX];
        uint8_t mClusterCommandSize;
        const uint8_t mSize;
    };

}

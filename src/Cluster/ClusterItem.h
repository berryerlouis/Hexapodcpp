#pragma once

#include <stdint.h>
#include "../Component/Communication/Frame/Frame.h"

namespace Cluster
{
    using Component::Frame;
    struct ClusterItem {
        ClusterItem() = default;
        explicit ClusterItem(const uint8_t commandId, const uint8_t expectedSize)
            : commandId(commandId)
            , expectedSize(expectedSize) {
        }
        ~ClusterItem() = default;


        uint8_t commandId = 0U;
        uint8_t expectedSize = 0U;
    };
} // namespace Cluster

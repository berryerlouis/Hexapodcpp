#pragma once

#include <stdint.h>

namespace Cluster
{
    struct ClusterItem {
        ClusterItem() = default;

        ~ClusterItem() = default;

        uint8_t commandId = 0U;
        uint8_t expectedSize = 0U;
    };
} // namespace Cluster

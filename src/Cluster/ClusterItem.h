#pragma once

#include "stdint.h"

namespace Cluster
{
    struct ClusterItem {
        ClusterItem() = default;

        ~ClusterItem() = default;

        uint8_t commandId;
        uint8_t expectedSize;
    };
}
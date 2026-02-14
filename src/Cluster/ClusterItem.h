#pragma once

#include <stdint.h>
#include "../Component/Communication/Frame/Frame.h"

namespace Cluster
{
    using Component::Frame;
    struct ClusterItem {
        ClusterItem() = default;

        ~ClusterItem() = default;

        uint8_t commandId = 0U;
        uint8_t expectedSize = 0U;
    };
} // namespace Cluster

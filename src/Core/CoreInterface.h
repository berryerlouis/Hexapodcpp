#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../Driver/Timer/Tick.h"
#include "Logger.h"
#include "Status.h"

namespace Core
{
    class CoreInterface {
    public:
        CoreInterface() = default;

        virtual Status Initialize(void) = 0;

        virtual void   Update(const uint64_t currentTime) = 0;
    };
} // namespace Core

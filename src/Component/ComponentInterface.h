#pragma once
#include "../Core/CoreInterface.h"
#include "../Misc/Logger/Logger.h"

namespace Component
{
    class ComponentInterface : public Core::CoreInterface {
    public:
        ComponentInterface() = default;

        ~ComponentInterface() = default;

        virtual Core::CoreStatus Initialize(void) = 0;

        virtual void Update(const uint64_t currentTime) = 0;
    };
}

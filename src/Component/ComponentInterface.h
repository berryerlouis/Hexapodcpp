#pragma once
#include "../Core/CoreInterface.h"
#include "../Misc/Logger/Logger.h"
#include "Observable.h"

namespace Component
{
    template<uint8_t MAX_OBSERVERS, class T>
    class ComponentInterface : public Core::CoreInterface, public Observable<MAX_OBSERVERS, T> {
    public:
        ComponentInterface() = default;

        ~ComponentInterface() = default;

        virtual Core::Status Initialize(void) = 0;

        virtual void Update(const uint64_t currentTime) = 0;
    };
}

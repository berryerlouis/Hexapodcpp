#pragma once
#include "../Core/CoreInterface.h"
#include "../Core/Observable.h"

namespace Component
{
#define LOG_COMPONENT_DEBUG(cmp, ...) LOG_GENERIC(DEBUG, COMPONENT, "[" cmp "] " __VA_ARGS__)
#define LOG_COMPONENT_INFO(cmp, ...) LOG_GENERIC(INFO, COMPONENT, "[" cmp "] " __VA_ARGS__)
#define LOG_COMPONENT_WARNING(cmp, ...) LOG_GENERIC(WARNING, COMPONENT, "[" cmp "] " __VA_ARGS__)
#define LOG_COMPONENT_ERROR(cmp, ...) LOG_GENERIC(ERROR, COMPONENT, "[" cmp "] " __VA_ARGS__)

    template<uint8_t MAX_OBSERVERS, class T>
    class ComponentInterface : public Core::CoreInterface, public Core::Observable<MAX_OBSERVERS, T> {
    public:
        ComponentInterface() = default;

        ~ComponentInterface() = default;

        virtual Core::Status Initialize(void) = 0;

        virtual void         Update(const uint64_t currentTime) = 0;
    };
} // namespace Component

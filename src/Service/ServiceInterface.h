#pragma once

#include "../Cluster/Constants.h"
#include "../Cluster/Frame/Frame.h"
#include "../Component/Communication/Communication.h"

namespace Service
{
#define LOG_SERVICE_DEBUG(...) LOG_GENERIC(DEBUG, SERVICE, __VA_ARGS__)
#define LOG_SERVICE_INFO(...) LOG_GENERIC(INFO, SERVICE, __VA_ARGS__)
#define LOG_SERVICE_WARNING(...) LOG_GENERIC(WARNING, SERVICE, __VA_ARGS__)
#define LOG_SERVICE_ERROR(...) LOG_GENERIC(ERROR, SERVICE, __VA_ARGS__)
    using namespace Cluster;

    class ServiceInterface : public Core::CoreInterface {
    public:
        ServiceInterface() = default;

        ~ServiceInterface() = default;
    };
} // namespace Service

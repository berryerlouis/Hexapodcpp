#pragma once

#include "../Component/Communication/Communication.h"
#include "../Cluster/Constants.h"
#include "../Cluster/Frame/Frame.h"

namespace Service
{
    using namespace Cluster;

    class ServiceInterface : public Core::CoreInterface {
    public:
        ServiceInterface() = default;

        ~ServiceInterface() = default;

        virtual Core::CoreStatus Initialize(void) = 0;

        virtual void Update(const uint64_t currentTime) = 0;
    };
}

#pragma once

#include "../../Cluster/Clusters/ClustersInterface.h"
#include "../../Driver/Socket/SocketInterface.h"
#include "../ComponentInterface.h"
#include "Frame/Frame.h"

namespace Component
{
    namespace Communication
    {
        enum CommunicationStruct {
            NO_CLIENT = 0U,
            CLIENT_CONNECTED = 1U,
        };

        using namespace Cluster;

        class CommunicationInterface
            : public ComponentInterface<1U, CommunicationStruct> {
        public:
            CommunicationInterface(void) = default;

            ~CommunicationInterface() = default;

            virtual Core::Status SendMessage(const Frame &message) = 0;
        };
    } // namespace Communication
} // namespace Component

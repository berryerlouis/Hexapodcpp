#pragma once

#include "../../Cluster/Frame/Frame.h"
#include "../../Driver/Socket/SocketInterface.h"
#include "../ComponentInterface.h"

namespace Component
{
    namespace Communication
    {
        enum CommunicationStruct {
            NO_CLIENT = 0U,
            CLIENT_CONNECTED = 1U,
        };

        using namespace Cluster;

        class CommunicationInterface : public ComponentInterface<1U, CommunicationStruct> {
        public:
            CommunicationInterface(void) = default;

            ~CommunicationInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void         Update(const uint64_t currentTime) = 0;

            virtual Core::Status SendMessage(const Frame &message) = 0;
        };
    } // namespace Communication
} // namespace Component

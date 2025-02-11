#pragma once

#include "../ComponentInterface.h"
#include "../../Cluster/Frame/Frame.h"

namespace Component
{
    namespace Communication
    {
        using namespace Cluster;

        class CommunicationInterface : public ComponentInterface<0U, nullptr_t> {
        public:
            CommunicationInterface(void) = default;

            ~CommunicationInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual Core::Status SendMessage(const Frame &message) = 0;
        };
    }
}

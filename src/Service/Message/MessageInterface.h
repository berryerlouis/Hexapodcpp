#pragma once

#include "../../Cluster/Frame/Frame.h"

namespace Service
{
    namespace Message
    {
        class MessageInterface {
        public:
            MessageInterface() = default;

            ~MessageInterface() = default;

            virtual void SendMessage(const Cluster::Frame &message) const = 0;
        };
    }
}
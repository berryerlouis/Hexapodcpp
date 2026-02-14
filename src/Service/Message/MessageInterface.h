#pragma once

#include "../../Component/Communication/Frame/Frame.h"

namespace Service
{
    namespace Message
    {
        using Component::Frame;
        class MessageInterface {
        public:
            MessageInterface() = default;

            virtual void SendMessage(const Frame &message) const = 0;
        };
    } // namespace Message
} // namespace Service

#pragma once

#include "MessageInterface.h"

#include "../../Component/Communication/CommunicationInterface.h"

namespace Service
{
    namespace Message
    {
        using namespace Component::Communication;

        class MessageListener : public MessageInterface {
        public:
            explicit MessageListener(CommunicationInterface &communication);

            ~MessageListener() = default;

            virtual void SendMessage(const Frame &message) const final override;

        private:
            Component::Communication::CommunicationInterface &mCommunication;
        };
    } // namespace Message
} // namespace Service

#pragma once

#include "MessageInterface.h"

#include "../../Component/Communication/CommunicationInterface.h"

namespace Service
{
    namespace Event
    {
        using namespace Component::Communication;

        class EventListener : public MessageInterface {
        public:
            EventListener(CommunicationInterface &communication);

            ~EventListener() = default;

            virtual void SendMessage(const Frame &message) const final override;

        private:
            Component::Communication::CommunicationInterface &mCommunication;
        };
    }
}

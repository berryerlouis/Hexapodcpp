#include "MessageListener.h"


namespace Service
{
    namespace Message
    {
        MessageListener::MessageListener(CommunicationInterface &communication) : mCommunication(communication) {
        }

        void MessageListener::SendMessage(const Frame &message) const {
            this->mCommunication.SendMessage(message);
        }
    } // namespace Message
} // namespace Service

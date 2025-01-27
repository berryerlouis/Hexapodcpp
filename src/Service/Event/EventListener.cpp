#include "EventListener.h"


namespace Service
{
    namespace Event
    {
        EventListener::EventListener(CommunicationInterface &communication) :
            mCommunication(communication) {
        }

        void EventListener::SendMessage(const Frame &message) const {
            this->mCommunication.SendMessage(message);
        }
    }

}

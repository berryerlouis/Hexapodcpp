#include "ServiceCommunication.h"

namespace Service
{
    namespace Communication
    {
        ServiceCommunication::ServiceCommunication(CommunicationInterface        &communication,
                                                   Clusters::ClustersInterface   &clusters,
                                                   Message::MessageInterface     &messageListener,
                                                   Event::EventListenerInterface &eventListener) :
            Service(COMMUNICATION,
                    1U,
                    messageListener,
                    eventListener),
            mClusters(clusters),
            mCommunication(communication) {
        }

        Core::Status ServiceCommunication::Initialize(void) {
            const Core::Status success = this->mCommunication.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->mCommunication.Attach(this);
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceCommunication::Update(const uint64_t currentTime) {
            this->mCommunication.Update(currentTime);
        }

        void ServiceCommunication::Notified(const CommunicationStruct &state) {
            this->SetEvent((state == CLIENT_CONNECTED) ? Event::Event::EVENT_COM_DONE : Event::Event::EVENT_COM_NONE);
        }

        void ServiceCommunication::DispatchEvent(const Event::Event &event) {
            (void) event;
        }
    } // namespace Communication
} // namespace Service

#include "ServiceCommunication.h"

namespace Service
{
    namespace Communication
    {
        ServiceCommunication::ServiceCommunication(CommunicationInterface          &communication,
                                                   Clusters::ClustersInterface     &clusters,
                                                   Message::MessageInterface       &messageListener,
                                                   Event::EventDispatcherInterface &eventDispatcher) :
            Service(COMMUNICATION,
                    1U,
                    messageListener,
                    eventDispatcher),
            mClusters(clusters),
            mCommunication(communication) {
        }

        Core::Status ServiceCommunication::Initialize(void) {
            const Core::Status success = this->mCommunication.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->GetEventDispatcher().AddListener(this);
                this->mCommunication.Attach(this);
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceCommunication::Update(const uint64_t currentTime) {
            this->mCommunication.Update(currentTime);
        }

        void ServiceCommunication::Notified(const CommunicationStruct &state) {
            this->DispatchEvent<CommunicationStruct>(EventType::EVENT_COM_UPDATE, state);
        }

        void ServiceCommunication::OnEvent(const Event::Event &event) {
            (void) event;
        }
    } // namespace Communication
} // namespace Service

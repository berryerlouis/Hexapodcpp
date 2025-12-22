#pragma once

#include "../../Cluster/Clusters/ClustersInterface.h"
#include "../../Component/Communication/CommunicationInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Communication
    {
        using namespace Component::Communication;

        class ServiceCommunication : public Service,
                                     Core::ObserverInterface<CommunicationStruct>,
                                     Event::EventListenerInterface {
        public:
            ServiceCommunication(CommunicationInterface          &communication,
                                 Clusters::ClustersInterface     &clusters,
                                 Message::MessageInterface       &messageListener,
                                 Event::EventDispatcherInterface &eventDispatcher);

            ~ServiceCommunication() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         Notified(const CommunicationStruct &state) final override;

            virtual void         OnEvent(const Event::Event &event) final override;

        private:
            Clusters::ClustersInterface &mClusters;

        protected:
            CommunicationInterface &mCommunication;
        };
    } // namespace Communication
} // namespace Service

#pragma once

#include "../../Component/Communication/CommunicationInterface.h"
#include "../../Cluster/Clusters/ClustersInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Communication
    {
        using namespace Component::Communication;

        class ServiceCommunication : public Service {
        public:
            ServiceCommunication(CommunicationInterface &communication, Clusters::ClustersInterface &clusters,
                                 Event::MessageInterface &messageListener);

            ~ServiceCommunication() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

        private:
            Clusters::ClustersInterface &mClusters;

        protected:
            CommunicationInterface &mCommunication;
        };
    }
}
#include "ServiceCommunication.h"

namespace Service
{
    namespace Communication
    {
        ServiceCommunication::ServiceCommunication(CommunicationInterface &communication,
                                                   Clusters::ClustersInterface &clusters,
                                                   Event::MessageInterface &messageListener) :
            Service(COMMUNICATION, 1U, messageListener)
            , mClusters(clusters)
            , mCommunication(communication) {
        }

        Core::Status ServiceCommunication::Initialize(void) {
            const Core::Status success = this->mCommunication.Initialize();
            if (Core::Status::CORE_OK == success) {
                this->mInitialized = true;
            }
            return (success);
        }

        void ServiceCommunication::Update(const uint64_t currentTime) {
            this->mCommunication.Update(currentTime);
        }

    } // namespace Communication
} // namespace Service

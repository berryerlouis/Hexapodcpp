#include "ServiceCommunication.h"
#include "../../Cluster/Battery/ClusterBattery.h"
#include "../../Cluster/Body/ClusterBody.h"
#include "../../Cluster/General/ClusterGeneral.h"
#include "../../Cluster/Imu/ClusterImu.h"
#include "../../Cluster/Proximity/ClusterProximity.h"
#include "../../Cluster/Servo/ClusterServo.h"

namespace Service
{
    namespace Communication
    {
        ServiceCommunication::ServiceCommunication(CommunicationInterface &communication,
                                                   Clusters::ClustersInterface &clusters,
                                                   Event::EventListenerInterface &eventListener) :
            Service(1U, eventListener), mClusters(clusters), mCommunication(communication) {
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

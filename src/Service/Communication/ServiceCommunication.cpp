#include "ServiceCommunication.h"
#include "../../Cluster/Battery/ClusterBattery.h"
#include "../../Cluster/General/ClusterGeneral.h"
#include "../../Cluster/Body/ClusterBody.h"
#include "../../Cluster/Imu/ClusterImu.h"
#include "../../Cluster/Proximity/ClusterProximity.h"
#include "../../Cluster/Servo/ClusterServo.h"

namespace Service
{
    namespace Communication
    {
        ServiceCommunication::ServiceCommunication(CommunicationInterface &communication,
                                                   Clusters::ClustersInterface &clusters)
            : Service(1U), mClusters(clusters), mCommunication(communication) {
        }

        Core::CoreStatus ServiceCommunication::Initialize(void) {
            return (this->mCommunication.Initialize());
        }

        void ServiceCommunication::Update(const uint64_t currentTime) {
            this->mCommunication.Update(currentTime);
        }

        void ServiceCommunication::SendMessage(Core::Event event) const {
            Frame response;
            bool success = false;
            switch (event.id) {
                case GENERAL: {
                    const Cluster::General::ClusterGeneral *clusterGeneral =
                            static_cast<Cluster::General::ClusterGeneral *>(this->mClusters.GetCluster(GENERAL));

                    if (event.value == Cluster::EGeneralCommands::MIN_EXECUTION_TIME) {
                        clusterGeneral->BuildFrameGetMinTime(response);
                    } else if (event.value == Cluster::EGeneralCommands::MAX_EXECUTION_TIME) {
                        clusterGeneral->BuildFrameGetMaxTime(response);
                    }
                    success = true;
                    break;
                }

                case IMU: {
                    //success = true;
                    break;
                }

                case PROXIMITY: {
                    const Cluster::Proximity::ClusterProximity *clusterProximity =
                            static_cast<Cluster::Proximity::ClusterProximity *>(this->mClusters.GetCluster(PROXIMITY));
                    clusterProximity->BuildFrameDistance(static_cast<Cluster::EProximityCommands>(event.value),
                                                         response);
                    success = true;
                    break;
                }

                case SERVO: {
                    //success = true;
                    break;
                }

                case BATTERY: {
                    const Cluster::Battery::ClusterBattery *clusterBattery =
                            static_cast<Cluster::Battery::ClusterBattery *>(this->mClusters.GetCluster(BATTERY));
                    clusterBattery->BuildFrameState(response);
                    success = true;
                    break;
                }

                case BODY: {
                    //success = true;
                    break;
                }

                default:
                    break;
            }

            if (true == success) {
                this->mCommunication.SendMessage(response);
            }
        }
    }
}

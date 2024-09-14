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
                                                   Clusters::ClustersInterface &clusters,
                                                   Event::EventListener &eventListener)
            : Service(1U, eventListener), mClusters(clusters), mCommunication(communication) {
        }

        Core::CoreStatus ServiceCommunication::Initialize(void) {
            return (this->mCommunication.Initialize());
        }

        void ServiceCommunication::Update(const uint64_t currentTime) {
            this->mCommunication.Update(currentTime);
        }

        void ServiceCommunication::DispatchEvent(SEvent &event) {
            Frame response;
            bool success = false;
            switch (event.id) {
                case EServices::GENERAL: {
                    const General::ClusterGeneral *clusterGeneral =
                            static_cast<General::ClusterGeneral *>(this->mClusters.GetCluster(EClusters::GENERAL));
                    const uint8_t serviceId = event.params[0U];
                    const uint16_t deltaTime = PTR_TO_UINT16(&event.params[1U]);
                    if (event.value == MIN_EXECUTION_TIME) {
                        clusterGeneral->BuildFrameGetMinTime(serviceId, deltaTime, response);
                    } else if (event.value == MAX_EXECUTION_TIME) {
                        clusterGeneral->BuildFrameGetMaxTime(serviceId, deltaTime, response);
                    }
                    success = true;
                    break;
                }

                case EServices::PROXIMITY: {
                    const Proximity::ClusterProximity *clusterProximity =
                            static_cast<Proximity::ClusterProximity *>(
                                this->mClusters.GetCluster(EClusters::PROXIMITY));
                    clusterProximity->BuildFrameDistance(static_cast<Proximity::SensorsId>(event.value), response);
                    success = true;
                    break;
                }

                case EServices::BATTERY: {
                    const Battery::ClusterBattery *clusterBattery =
                            static_cast<Battery::ClusterBattery *>(this->mClusters.GetCluster(EClusters::BATTERY));
                    const uint16_t voltage = PTR_TO_UINT16(&event.params[0U]);
                    clusterBattery->BuildFrameState(event.value, voltage, response);
                    success = true;
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

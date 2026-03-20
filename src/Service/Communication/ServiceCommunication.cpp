#include "ServiceCommunication.h"

namespace Service
{
    namespace Communication
    {
        ServiceCommunication::ServiceCommunication(CommunicationInterface          &communication,
                                                   Clusters::ClustersInterface     &clusters,
                                                   Message::MessageInterface       &messageListener,
                                                   Event::EventDispatcherInterface &eventDispatcher)
            : Service(COMMUNICATION, 5U, messageListener, eventDispatcher)
            , mClusters(clusters)
            , mCommunication(communication) {
        }

        Core::Status ServiceCommunication::Initialize() {
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

            constexpr uint8_t MAX_MESSAGES_PER_UPDATE = 8U;
            uint8_t           processedMessages = 0U;
            Frame message;
            while (processedMessages < MAX_MESSAGES_PER_UPDATE &&
                   this->mCommunication.GetMessage(message) == Core::Status::CORE_OK) {
                this->DispatchToClusters(message);
                processedMessages += 1U;
            }
        }

        void ServiceCommunication::DispatchToClusters(const Frame &message) {
            const uint8_t frameClusterID = message.GetClusterId();
            Frame         response;
            if (frameClusterID < NB_CLUSTERS) {
                auto *const cluster =
                        this->mClusters.GetCluster(static_cast<EClusters>(frameClusterID));
                if (cluster != nullptr) {
                    if (cluster->Execute((Frame &) message, response) != Core::Status::CORE_OK) {
                        response.Build(frameClusterID, GENERIC);
                        response.Set1ByteParam(Core::Status::CORE_ERROR_ARGUMENT);
                        LOG_COMPONENT_ERROR("Communication",
                                            "RX => "
                                            "CORE_ERROR_"
                                            "ARGUMENT");
                    }
                } else {
                    response.Build(frameClusterID, GENERIC);
                    response.Set1ByteParam(Core::Status::CORE_ERROR_NULLPTR);
                    LOG_COMPONENT_ERROR("Communication",
                                        "RX => "
                                        "CORE_ERROR_NULLPTR");
                }
            } else {
                response.Build(0xFFU, GENERIC);
                response.Set1ByteParam(Core::Status::CORE_ERROR_UNKNOWN_CLUSTER);
                LOG_COMPONENT_ERROR("Communication",
                                    "RX => "
                                    "CORE_ERROR_UNKNOWN_"
                                    "CLUSTER");
            }

            this->SendMessage(response);
        }

        void ServiceCommunication::Notified(const CommunicationStruct &state) {
            this->DispatchEvent<CommunicationStruct>(EventType::EVENT_COM_UPDATE, state);
        }

        void ServiceCommunication::OnEvent(const Event::Event &event) {
            (void) event;
        }
    } // namespace Communication
} // namespace Service

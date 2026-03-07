#include "Communication.h"


namespace Component
{
    namespace Communication
    {
        Communication::Communication(Socket::SocketInterface<1U, Socket::SocketStruct> &socket,
                                     Clusters::ClustersInterface                       &clusters,
                                     Led::LedInterface                                 &ledStatus)
            : mSocket(socket)
            , mClusters(clusters)
            , mLedStatus(ledStatus)
            , mBufferTx{0U} {
        }

        Core::Status Communication::Initialize() {
            this->mSocket.Attach(this);
            LOG_COMPONENT_DEBUG("Communication", "Initialized.");
            return (this->mLedStatus.Initialize());
        }

        void Communication::Notified(const Socket::SocketStruct &state) {
            this->Notify(state == Socket::SocketStruct::CLIENT_CONNECTED ? CLIENT_CONNECTED
                                                                         : NO_CLIENT);
        }

        void Communication::Update(const uint64_t currentTime) {
            this->mSocket.Update(currentTime);
            if (this->mSocket.HasNewFrame()) {
                // this->mLedStatus.On();
                Frame              request;
                Frame              response;
                const Core::Status parsedStatus = Protocol::Decode(
                        const_cast<const char *>(this->mSocket.ReadIncomingData()), request);
                if (parsedStatus == Core::Status::CORE_OK) {
                    const uint8_t frameClusterID = request.GetClusterId();
                    if (frameClusterID < NB_CLUSTERS) {
                        auto *const cluster =
                                this->mClusters.GetCluster(static_cast<EClusters>(frameClusterID));
                        if (cluster != nullptr) {
                            if (cluster->Execute(request, response) != Core::Status::CORE_OK) {
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
                } else {
                    response.Build(0xFFU, GENERIC);
                    response.Set1ByteParam(parsedStatus);
                    LOG_COMPONENT_ERROR("Communication",
                                        "RX => Parsing error %s",
                                        Core::StatusToString(parsedStatus).c_str());
                }
                this->SendMessage(response);
                // this->mLedStatus.Off();
            }
        }

        Core::Status Communication::SendMessage(const Frame &message) {
            const size_t size = Protocol::Encode(message, const_cast<char *>(this->mBufferTx));
            if (size != 0U) {
                this->mSocket.Send(const_cast<const char *>(this->mBufferTx), size);
                return (Core::Status::CORE_OK);
            }
            return (Core::Status::CORE_ERROR);
        }
    } // namespace Communication
} // namespace Component

#include "Communication.h"


namespace Component
{
    namespace Communication
    {
        Communication::Communication(Socket::SocketInterface<1U, Socket::SocketStruct> &socket,
                                     Clusters::ClustersInterface &clusters,
                                     Led::LedInterface &ledStatus) :
            mSocket(socket),
            mClusters(clusters),
            mLedStatus(ledStatus),
            mBufferRx{0U},
            mBufferTx{0U},
            mIndexBufferRx(0U),
            mBeginIncomingFrame(false) {
        }

        Core::Status Communication::Initialize(void) {
            this->mSocket.Attach(this);
            LOG_COMPONENT_DEBUG("Communication", "Initialized.");
            return (this->mLedStatus.Initialize());
        }

        void Communication::Notified(const Socket::SocketStruct &state) {
            this->Notify(state == Socket::SocketStruct::CLIENT_CONNECTED ? CLIENT_CONNECTED : NO_CLIENT);
        }

        void Communication::Update(const uint64_t currentTime) {
            this->mSocket.Update(currentTime);
            if (true == this->ReceivedStringFrame()) {
                //this->mLedStatus.On();
                Frame request;
                Frame response;
                const Core::Status parsedStatus =
                        Protocol::Decode(const_cast<const char *>(this->mBufferRx), request);
                if (parsedStatus == Core::Status::CORE_OK) {
                    const uint8_t frameClusterID = request.GetClusterId();
                    if (frameClusterID < NB_CLUSTERS) {
                        const auto cluster = this->mClusters.GetCluster(static_cast<EClusters>(frameClusterID));
                        if (cluster != nullptr) {
                            if (cluster->Execute(request, response) != Core::Status::CORE_OK) {
                                response.Build(frameClusterID, GENERIC);
                                response.Set1ByteParam(Core::Status::CORE_ERROR_ARGUMENT);
                                LOG_COMPONENT_ERROR("Communication", "RX => CORE_ERROR_ARGUMENT");
                            }
                        } else {
                            response.Build(frameClusterID, GENERIC);
                            response.Set1ByteParam(Core::Status::CORE_ERROR_NULLPTR);
                            LOG_COMPONENT_ERROR("Communication", "RX => CORE_ERROR_NULLPTR");
                        }
                    } else {
                        response.Build(0xFFU, GENERIC);
                        response.Set1ByteParam(Core::Status::CORE_ERROR_UNKNOWN_CLUSTER);
                        LOG_COMPONENT_ERROR("Communication", "RX => CORE_ERROR_UNKNOWN_CLUSTER");
                    }
                } else {
                    response.Build(0xFFU, GENERIC);
                    response.Set1ByteParam(parsedStatus);
                    LOG_COMPONENT_ERROR("Communication", "RX => Parsing error %s",
                                        Core::StatusToString(parsedStatus).c_str());
                }
                this->SendMessage(response);
                //this->mLedStatus.Off();
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

        bool Communication::ReceivedStringFrame(void) {
            uint8_t nbData = this->mSocket.DataAvailable();
            while (nbData != 0U) {
                nbData--;
                const volatile uint8_t rc = this->mSocket.Read();
                if (rc == '<') {
                    this->mBeginIncomingFrame = true;
                    this->mIndexBufferRx = 0U;
                } else if (rc != '>') {
                    if ((rc >= '0' && rc <= '9') || (rc >= 'A' && rc <= 'F')) {
                        if (this->mIndexBufferRx < sizeof(this->mBufferRx) - 1) {
                            this->mBufferRx[this->mIndexBufferRx++] = rc;
                        } else {
                            this->mIndexBufferRx = 0U;
                            this->mBeginIncomingFrame = false;
                        }
                    } else {
                        this->mIndexBufferRx = 0U;
                        this->mBeginIncomingFrame = false;
                    }
                } else {
                    if (this->mBeginIncomingFrame == true && this->mIndexBufferRx >= 6U &&
                        (this->mIndexBufferRx & 0x01U) == 0U) {
                        this->mBufferRx[this->mIndexBufferRx] = '\0';
                        this->mIndexBufferRx = 0U;
                        this->mBeginIncomingFrame = false;
                        return (true);
                    }
                    this->mIndexBufferRx = 0U;
                    this->mBeginIncomingFrame = false;
                }
            }
            return (false);
        }
    } // namespace Communication
} // namespace Component

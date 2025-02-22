#include "Communication.h"


namespace Component
{
    namespace Communication
    {
        Communication::Communication(Socket::SocketInterface<1U, Socket::SocketStruct> &socket,
                                     Clusters::ClustersInterface &clusters,
                                     Led::LedInterface &ledStatus) :
            mSocket(socket), mClusters(clusters), mLedStatus(ledStatus), mBufferRx{0U}, mBufferTx{0U},
            mIndexBufferRx(0U),
            mBeginIncomingFrame(false) {
        }

        Core::Status Communication::Initialize(void) {
            this->mSocket.Attach(this);
            return (this->mLedStatus.Initialize());
        }

        void Communication::Notified(const Socket::SocketStruct &state) {
            this->Notify(state == Socket::SocketStruct::CLIENT_CONNECTED ? CLIENT_CONNECTED : NO_CLIENT);
        }

        void Communication::Update(const uint64_t currentTime) {
            this->mSocket.Update(currentTime);
            if (true == this->ReceivedStringFrame()) {
                this->mLedStatus.On();
                Frame request;
                Frame response;
                const Core::Status parsedStatus =
                        Protocol::Decode(const_cast<const char *>(this->mBufferRx), request);
                if (parsedStatus == Core::Status::CORE_OK) {
                    bool success = false;
                    const uint8_t frameClusterID = request.clusterId;
                    if (frameClusterID < NB_CLUSTERS) {
                        const auto cluster = this->mClusters.GetCluster(static_cast<EClusters>(frameClusterID));
                        if (cluster != nullptr) {
                            if (cluster->Execute(request, response) == Core::Status::CORE_OK) {
                                success = true;
                            }
                        }
                    }
                    if (success == false) {
                        response.clusterId = frameClusterID;
                        response.commandId = static_cast<uint8_t>(GENERIC);
                        response.nbParams = 1U;
                        response.params[0U] = false;
                    }
                } else {
                    response.clusterId = 0xFFU;
                    response.commandId = static_cast<uint8_t>(GENERIC);
                    response.nbParams = 1U;
                    response.params[0U] = parsedStatus;
                }
                this->SendMessage(response);
                this->mLedStatus.Off();
            }
        }

        Core::Status Communication::SendMessage(const Frame &message) {
            const size_t size = Protocol::Encode(message, const_cast<char *>(this->mBufferTx));
            if (size != 0) {
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

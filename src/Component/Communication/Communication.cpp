#include "Communication.h"


namespace Component
{
    namespace Communication
    {
        Communication::Communication(Uart::UartInterface &uart, Clusters::ClustersInterface &clusters,
                                     Led::LedInterface &ledStatus) :
            mUart(uart), mClusters(clusters), mLedStatus(ledStatus), mBufferRx{0U}, mBufferTx{0U}, mIndexBufferRx(0U),
            mBeginIncomingFrame(false) {
        }

        Core::Status Communication::Initialize(void) {
            return (this->mLedStatus.Initialize());
        }

        void Communication::Update(const uint64_t currentTime) {
            this->mUart.Update(currentTime);
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
                        response.params[0] = false;
                    }
                } else {
                    response.clusterId = 0xFFU;
                    response.commandId = static_cast<uint8_t>(GENERIC);
                    response.nbParams = 1U;
                    response.params[0] = parsedStatus;
                }
                this->SendMessage(response);
                this->mLedStatus.Off();
            }
        }

        Core::Status Communication::SendMessage(Frame &message) {
            const size_t size = Protocol::Encode(message, const_cast<char *>(this->mBufferTx));
            if (size != 0) {
                this->mUart.Send(const_cast<const char *>(this->mBufferTx), size);
                return (Core::Status::CORE_OK);
            }
            return (Core::Status::CORE_ERROR);
        }

        bool Communication::ReceivedStringFrame(void) {
            uint8_t nbData = this->mUart.DataAvailable();
            while (nbData > 0U) {
                nbData--;
                const volatile uint8_t rc = this->mUart.Read();
                if (rc == '<') {
                    this->mBeginIncomingFrame = true;
                    this->mIndexBufferRx = 0U;
                } else if (rc != '>') {
                    if ((rc >= 48U && rc <= 57U) || (rc >= 65U && rc <= 70U)) {
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

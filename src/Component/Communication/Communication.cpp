#include "Communication.h"


namespace Component
{
    namespace Communication
    {
        Communication::Communication(Uart::UartInterface &uart, Clusters::ClustersInterface &clusters,
                                     Led::LedInterface &ledStatus)
            : mUart(uart), mClusters(clusters), mLedStatus(ledStatus), mBufferRx{0U}, mIndexBufferRx(0U) {
        }

        Core::CoreStatus Communication::Initialize(void) {
            return (this->mLedStatus.Initialize());
        }

        void Communication::Update(const uint64_t currentTime) {
            (void) currentTime;
            if (true == this->ReceivedStringFrame()) {
                this->mLedStatus.On();
                Frame request;
                Frame response;
                const Protocol::ProtocolStatus parsedStatus = Protocol::Decode(this->mBufferRx, request);
                if (parsedStatus == Protocol::ProtocolStatus::NO_ERROR) {
                    uint8_t frameClusterID = request.clusterId;
                    const auto cluster = this->mClusters.GetCluster(static_cast<EClusters>(frameClusterID));

                    if (cluster != nullptr) {
                        if (true == cluster->Execute(request, response)) {
                            this->SendMessage(response);
                        } else {
                            response.clusterId = request.clusterId;
                            response.commandId = static_cast<uint8_t>(GENERIC);
                            response.nbParams = 1U;
                            response.params[0] = false;
                            this->SendMessage(response);
                        }
                        response.Reset();
                    } else {
                        response.clusterId = request.clusterId;
                        response.commandId = static_cast<uint8_t>(GENERIC);
                        response.nbParams = 1U;
                        response.params[0] = false;
                        this->SendMessage(response);
                    }
                } else {
                    response.clusterId = 0xFFU;
                    response.commandId = static_cast<uint8_t>(GENERIC);
                    response.nbParams = 1U;
                    response.params[0] = parsedStatus;
                    this->SendMessage(response);
                }
                this->mLedStatus.Off();
            }
        }

        Core::CoreStatus Communication::SendMessage(Frame &message) {
            uint8_t buffer[50U] = {0U};

            const size_t size = Protocol::Encode(message, buffer);

            if (size != 0) {
                this->mUart.Send(buffer, size);
                return (Core::CoreStatus::CORE_OK);
            }
            return (Core::CoreStatus::CORE_ERROR);
        }

        bool Communication::ReceivedStringFrame(void) {
            while (this->mUart.DataAvailable() > 0) {
                const char rc = this->mUart.Read();
                if (rc == '<') {
                    this->mIndexBufferRx = 0U;
                } else if (rc != '>') {
                    this->mBufferRx[this->mIndexBufferRx] = rc;
                    this->mIndexBufferRx++;
                } else {
                    this->mBufferRx[this->mIndexBufferRx] = '\0';
                    this->mIndexBufferRx++;
                    return (true);
                }
            }
            return (false);
        }
    }
}

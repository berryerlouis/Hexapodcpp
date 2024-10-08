#include "Communication.h"


namespace Component
{
    namespace Communication
    {
#define REJECT_UNKNOWN_CHARACTER(value) \
        ((value == 60U || value == 62U) ||  \
        (value >= 48U && value <= 57U) ||  \
        (value >= 65U && value <= 70U) ||  \
        (value >= 97U && value <= 102U))

        Communication::Communication(Uart::UartInterface &uart, Clusters::ClustersInterface &clusters,
                                     Led::LedInterface &ledStatus)
            : mUart(uart)
              , mClusters(clusters)
              , mLedStatus(ledStatus)
              , mBufferRx{0U}
              , mIndexBufferRx(0U)
              , mBeginIncomingFrame(false) {
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
                const Core::CoreStatus parsedStatus = Protocol::Decode(this->mBufferRx, request);
                if (parsedStatus == Core::CoreStatus::CORE_OK) {
                    uint8_t frameClusterID = request.clusterId;
                    const auto cluster = this->mClusters.GetCluster(static_cast<EClusters>(frameClusterID));

                    if (cluster != nullptr) {
                        if (cluster->Execute(request, response) == Core::CoreStatus::CORE_OK) {
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
            constexpr char buffer[50U] = {0U};

            const size_t size = Protocol::Encode(message, buffer);

            if (size != 0) {
                this->mUart.Send(buffer, size);
                return (Core::CoreStatus::CORE_OK);
            }
            return (Core::CoreStatus::CORE_ERROR);
        }

        bool Communication::ReceivedStringFrame(void) {
            if (this->mUart.DataAvailable() > 0U) {
                const uint8_t rc = this->mUart.Read();
                if (REJECT_UNKNOWN_CHARACTER(rc)) {
                    if (rc == '<') {
                        this->mBeginIncomingFrame = true;
                        this->mIndexBufferRx = 0U;
                    } else if (rc != '>') {
                        this->mBufferRx[this->mIndexBufferRx] = rc;
                        this->mIndexBufferRx++;
                    } else {
                        if (this->mBeginIncomingFrame == true) {
                            this->mBufferRx[this->mIndexBufferRx] = '\0';
                            return (true);
                        }
                        this->mIndexBufferRx = 0U;
                        this->mBeginIncomingFrame = false;
                        return (false);
                    }
                } else {
                    this->mIndexBufferRx = 0U;
                    this->mBeginIncomingFrame = false;
                    return (false);
                }
            }
            return (false);
        }
    }
}

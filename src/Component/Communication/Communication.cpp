#include "Communication.h"


namespace Component
{
    namespace Communication
    {
        Communication::Communication(Socket::SocketInterface<1U, Socket::SocketStruct> &socket,
                                     Led::LedInterface                                 &ledStatus)
            : mSocket(socket)
            , mLedStatus(ledStatus)
            , mBufferTx{0U}
            , mReceivedFrames{}
            , mRxReadIndex(0U)
            , mRxWriteIndex(0U)
            , mRxCount(0U) {
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

            while (this->mSocket.HasNewFrame()) {
                // this->mLedStatus.On();
                Frame              request;
                const Core::Status parsedStatus = Protocol::Decode(
                        const_cast<const char *>(this->mSocket.ReadIncomingData()), request);
                if (parsedStatus == Core::Status::CORE_OK) {
                    if (this->PushReceivedFrame(request) != Core::Status::CORE_OK) {
                        LOG_COMPONENT_WARNING("Communication",
                                              "RX queue full, drop frame c=%u cmd=%u",
                                              request.GetClusterId(),
                                              request.GetCommandId());
                    }
                } else {
                    LOG_COMPONENT_WARNING("Communication", "Failed to decode incoming frame.");
                }
                // this->mLedStatus.Off();
            }
        }


        Core::Status Communication::GetMessage(Frame &message) {
            return this->PopReceivedFrame(message);
        }

        Core::Status Communication::PushReceivedFrame(const Frame &frame) {
            if (this->mRxCount >= RX_QUEUE_CAPACITY) {
                return Core::Status::CORE_ERROR_OVERLOAD;
            }

            this->mReceivedFrames[this->mRxWriteIndex] = frame;
            this->mRxWriteIndex = (this->mRxWriteIndex + 1U) % RX_QUEUE_CAPACITY;
            this->mRxCount += 1U;
            return Core::Status::CORE_OK;
        }

        Core::Status Communication::PopReceivedFrame(Frame &frame) {
            if (this->mRxCount == 0U) {
                return Core::Status::CORE_ERROR;
            }

            frame = this->mReceivedFrames[this->mRxReadIndex];
            this->mRxReadIndex = (this->mRxReadIndex + 1U) % RX_QUEUE_CAPACITY;
            this->mRxCount -= 1U;
            return Core::Status::CORE_OK;
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

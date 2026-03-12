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
            , mReceivedFrames() {
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
                Frame              response;
                const Core::Status parsedStatus = Protocol::Decode(
                        const_cast<const char *>(this->mSocket.ReadIncomingData()), request);
                if (parsedStatus == Core::Status::CORE_OK) {
                    this->mReceivedFrames.push_back(request);
                }
                // this->mLedStatus.Off();
            }
        }


        Core::Status Communication::GetMessage(Frame &message) {
            if (!this->mReceivedFrames.empty()) {
                message = this->mReceivedFrames.front();
                this->mReceivedFrames.erase(this->mReceivedFrames.begin());
                return Core::Status::CORE_OK;
            }
            return Core::Status::CORE_ERROR;
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

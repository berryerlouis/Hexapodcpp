#pragma once

#include <array>
#include "../../Driver/Socket/SocketInterface.h"
#include "../Led/LedInterface.h"
#include "CommunicationInterface.h"
#include "Decoding/Protocol.h"


namespace Component
{
    namespace Communication
    {
        using namespace Driver;
        using namespace Component::Decoding;

        class Communication : public CommunicationInterface,
                              Core::ObserverInterface<Socket::SocketStruct> {
        public:
            Communication(Socket::SocketInterface<1U, Socket::SocketStruct> &socket,
                          Led::LedInterface                                 &ledStatus);

            ~Communication() = default;


            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual Core::Status SendMessage(const Frame &message) final override;

            virtual Core::Status GetMessage(Frame &message) final override;

            virtual void         Notified(const Socket::SocketStruct &state) final override;

        private:
            static constexpr size_t RX_QUEUE_CAPACITY = 16U;

            Core::Status PushReceivedFrame(const Frame &frame);

            Core::Status PopReceivedFrame(Frame &frame);

            Socket::SocketInterface<1U, Socket::SocketStruct> &mSocket;
            Led::LedInterface                                 &mLedStatus;
            volatile char                                      mBufferTx[50U];
            std::array<Frame, RX_QUEUE_CAPACITY>              mReceivedFrames;
            size_t                                             mRxReadIndex;
            size_t                                             mRxWriteIndex;
            size_t                                             mRxCount;
        };
    } // namespace Communication
} // namespace Component

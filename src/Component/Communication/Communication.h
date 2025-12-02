#pragma once

#include "CommunicationInterface.h"
#include "../Led/LedInterface.h"
#include "../../Driver/Socket/SocketInterface.h"
#include "../../Cluster/Decoding/Protocol.h"
#include "../../Cluster/Clusters/ClustersInterface.h"
#include "../../Cluster/Constants.h"


namespace Component
{
    namespace Communication
    {
        using namespace Driver;
        using namespace Cluster::Decoding;

        class Communication : public CommunicationInterface, Core::ObserverInterface<Socket::SocketStruct> {
        public:
            Communication(Socket::SocketInterface<1U, Socket::SocketStruct> &socket,
                          Clusters::ClustersInterface &clusters,
                          Led::LedInterface &ledStatus);

            ~Communication() = default;


            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual Core::Status SendMessage(const Frame &message) final override;

            virtual void Notified(const Socket::SocketStruct &state) final override;

        private:
            bool ReceivedStringFrame(void);

            Socket::SocketInterface<1U, Socket::SocketStruct> &mSocket;
            Clusters::ClustersInterface &mClusters;
            Led::LedInterface &mLedStatus;
            volatile char mBufferRx[50U];
            volatile char mBufferTx[50U];
            uint8_t mIndexBufferRx;
            bool mBeginIncomingFrame;
        };
    }
}
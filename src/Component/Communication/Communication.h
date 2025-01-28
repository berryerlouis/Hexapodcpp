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

        class Communication : public CommunicationInterface {
        public:
            Communication(Socket::SocketInterface &socket, Clusters::ClustersInterface &clusters,
                          Led::LedInterface &ledStatus);

            ~Communication() = default;


            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual Core::Status SendMessage(const Frame &message) final override;

        private:
            bool ReceivedStringFrame(void);

            Socket::SocketInterface &mSocket;
            Clusters::ClustersInterface &mClusters;
            Led::LedInterface &mLedStatus;
            volatile char mBufferRx[50U];
            volatile char mBufferTx[50U];
            uint8_t mIndexBufferRx;
            bool mBeginIncomingFrame;
        };
    }
}

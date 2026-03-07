#pragma once

#include "../../Cluster/Clusters/ClustersInterface.h"
#include "../../Cluster/Constants.h"
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
                          Clusters::ClustersInterface                       &clusters,
                          Led::LedInterface                                 &ledStatus);

            ~Communication() = default;


            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual Core::Status SendMessage(const Frame &message) final override;

            virtual void         Notified(const Socket::SocketStruct &state) final override;

        private:
            Socket::SocketInterface<1U, Socket::SocketStruct> &mSocket;
            Clusters::ClustersInterface                       &mClusters;
            Led::LedInterface                                 &mLedStatus;
            volatile char                                      mBufferTx[50U];
        };
    } // namespace Communication
} // namespace Component

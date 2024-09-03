#pragma once

#include "CommunicationInterface.h"
#include "../Led/LedInterface.h"
#include "../../Driver/Uart/UartInterface.h"
#include "../../Cluster/Decoding/Protocol.h"
#include "../../Cluster/Clusters/ClustersInterface.h"
#include "../../Cluster/Constants.h"


namespace Component {
namespace Communication {
using namespace Driver;
using namespace Cluster::Decoding;

class Communication : public CommunicationInterface {
public:
    Communication( Uart::UartInterface &uart, Clusters::ClustersInterface &clusters, Led::LedInterface &ledStatus );
    ~Communication() = default;


    virtual Core::CoreStatus Initialize( void ) final override;
    virtual void Update( const uint64_t currentTime ) final override;

    virtual Core::CoreStatus SendMessage( Frame &message ) final override;

private:
    bool ReceivedStringFrame( void );

    Uart::UartInterface &mUart;
    Clusters::ClustersInterface &mClusters;
    Led::LedInterface &mLedStatus;
    char mBufferRx[100U];
    uint8_t mIndexBufferRx;
};
}
}

#pragma once

#include "CommunicationInterface.h"
#include "../drv/Uart.h"
#include "../clu/ClustersInterface.h"
#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../clu/Protocol.h"
#include "../cmp/LedInterface.h"


namespace Component {
using namespace Driver;
using namespace Clusters;
class Communication : public CommunicationInterface {
public:
	Communication( UartInterface &uart, ClustersInterface &clusters, LedInterface &ledStatus );
	~Communication() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint32_t currentTime ) final override;

	virtual Core::CoreStatus Send( Clusters::Frame &message ) final override;

private:
	bool ReceivedStringFrame( void );

	UartInterface &mUart;
	ClustersInterface &mClusters;
	LedInterface &mLedStatus;
	char mBufferRx[100U];
	uint8_t mIndexBufferRx;
};
}

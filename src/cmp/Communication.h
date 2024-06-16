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
using namespace Cluster;
class Communication : public CommunicationInterface {
public:
	Communication(UartInterface &uart, const ClustersInterface &clusters, LedInterface &ledStatus);
	~Communication() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	virtual bool Send(Frame &message) final override;

private:
	bool ReceivedStringFrame(void);

	UartInterface &mUart;
	const ClustersInterface &mClusters;
	LedInterface &mLedStatus;
	char mBufferRx[100U];
	uint8_t mIndexBufferRx;
};
}

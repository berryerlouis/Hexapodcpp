#pragma once

#include "ComponentInterface.h"
#include "../drv/Uart.h"
#include "../clu/Clusters.h"
#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../clu/Protocol.h"
#include "../cmp/Led.h"


namespace Component {
using namespace Driver;
using namespace Cluster;
class Communication : public ComponentInterface {
public:
	Communication(UartInterface &uart, const Clusters &clusters, Led &ledStatus);
	~Communication() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	bool Send(Frame &message);

private:
	bool ReceivedStringFrame(void);

	UartInterface &mUart;
	const Clusters &mClusters;
	Led &mLedStatus;
	char mBufferRx[100U];
	uint8_t mIndexBufferRx;
};
}

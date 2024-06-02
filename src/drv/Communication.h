#pragma once

#include "../clu/Clusters.h"
#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../clu/Protocol.h"
#include "../cmp/Led.h"
#include <arduino.h>

class Communication {
public:
	Communication(const Clusters&clusters, Led&ledStatus);
	~Communication() = default;

	void Update();
	static bool Send(Frame&message);

private:
	bool ReceivedStringFrame(void);

	const Clusters&mClusters;
	Led&mLedStatus;
	Protocol mProtocol;
};

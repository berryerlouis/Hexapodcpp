#pragma once

#include "ComponentInterface.h"
#include "../clu/Frame.h"

namespace Component {
class CommunicationInterface : public ComponentInterface {
public:

	CommunicationInterface(void) = default;
	~CommunicationInterface()    = default;

	virtual Core::CoreStatus Initialize(void)       = 0;
	virtual void Update(const uint32_t currentTime) = 0;

	virtual Core::CoreStatus Send(Cluster::Frame &message) = 0;
};
}

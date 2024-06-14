#pragma once

#include "ComponentInterface.h"
#include "../clu/Frame.h"

namespace Component {
class CommunicationInterface : public ComponentInterface {
public:


	CommunicationInterface(void) = default;
	~CommunicationInterface()    = default;

	virtual bool Send(Cluster::Frame &message) = 0;
};
}

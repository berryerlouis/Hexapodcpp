#pragma once

#include "../cmp/CommunicationInterface.h"
#include "../srv/Service.h"
#include "../srv/ServiceBattery.h"
#include "../srv/ServiceControl.h"
#include "../srv/ServiceOrientation.h"
#include "../srv/ServiceProximity.h"


using namespace Cluster;
using namespace Component;
using namespace Driver;

class Services : public ServiceInterface, public ServiceMediatorInterface {
public:
	static const uint8_t NB_SERVICES = 4U;
	Services(
		CommunicationInterface &communication,
		ServiceControl &serviceControl,
		ServiceProximity &serviceProximity,
		ServiceOrientation &serviceOrientation,
		ServiceBattery &serviceBattery);

	~Services() = default;

	virtual Core::CoreStatus Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	Service *GetService(const uint8_t serviceId) const;
	virtual void SendFrame(Frame &message) const override;

private:
	CommunicationInterface &mCommunication;
	Service *mServices[NB_SERVICES];
};

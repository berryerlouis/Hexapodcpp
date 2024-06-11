#pragma once

#include "../srv/Service.h"
#include "../srv/ServiceBattery.h"
#include "../srv/ServiceControl.h"
#include "../srv/ServiceOrientation.h"
#include "../srv/ServiceProximity.h"

class Services : public ServiceInterface {
public:
	static const uint8_t NB_SERVICES = 4U;
	Services(
		ServiceControl &serviceControl,
		ServiceProximity &serviceProximity,
		ServiceOrientation &serviceOrientation,
		ServiceBattery &serviceBattery);

	~Services() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	Service *GetService(const uint8_t serviceId) const;

private:
	Service *mServices[NB_SERVICES];
};

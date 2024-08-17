#pragma once

#include "../cmp/CommunicationInterface.h"
#include "../srv/Constants.h"
#include "../srv/Service.h"
#include "../srv/ServiceBattery.h"
#include "../srv/ServiceControl.h"
#include "../srv/ServiceDisplay.h"
#include "../srv/ServiceGeneral.h"
#include "../srv/ServiceOrientation.h"
#include "../srv/ServiceProximity.h"


using namespace Clusters;
using namespace Component;
using namespace Driver;


struct ServiceItem
{
	EServices serviceId;
	Service * service;

	ServiceItem()  = default;
	~ServiceItem() = default;
};

class Services : public ServiceInterface, public ServiceMediatorInterface {
public:
	Services(
		CommunicationInterface &communication,
		ServiceGeneral &serviceGeneral,
		ServiceControl &serviceControl,
		ServiceProximity &serviceProximity,
		ServiceOrientation &serviceOrientation,
		ServiceBattery &serviceBattery,
		ServiceDisplay &serviceDisplay );

	~Services() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual void SendFrame( Frame &message ) const override;
	virtual void DisplayBatteryLevel( Component::BatteryState state ) override;
	virtual void DisplayProximitySensor( Component::SensorProximityInterface::SensorsId sensor ) override;

private:

	Service *Get( const EServices serviceId );

	CommunicationInterface & mCommunication;
	ServiceItem mServices[NB_SERVICES];
};

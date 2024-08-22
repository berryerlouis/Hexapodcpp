#pragma once

#include "../../Component/Communication/CommunicationInterface.h"
#include "../Constants.h"
#include "../Service.h"
#include "../Battery/ServiceBattery.h"
#include "../Control/ServiceControl.h"
#include "../Display/ServiceDisplay.h"
#include "../General/ServiceGeneral.h"
#include "../Orientation/ServiceOrientation.h"
#include "../Proximity/ServiceProximity.h"

namespace Service {
namespace Services {
using namespace Component::Communication;
using namespace ::Service::General;
using namespace ::Service::Control;
using namespace ::Service::Proximity;
using namespace ::Service::Orientation;
using namespace ::Service::Battery;
using namespace ::Service::Display;

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

private:

	Service *Get( const EServices serviceId );

	CommunicationInterface & mCommunication;
	ServiceItem mServices[NB_SERVICES];
};
}
}

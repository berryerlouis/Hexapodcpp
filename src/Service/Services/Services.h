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
#include "../../Core/Event/Event.h"
#include "../../Core/Event/EventInterface.h"

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

class Services : public ServiceInterface, public ServiceMediatorInterface, public Core::EventInterface {
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

	virtual void SendFrame( Frame &message ) const final override;

	virtual void AddEvent( const Core::EventId &event ) final override;

private:

	Service *Get( const EServices serviceId );

	CommunicationInterface & mCommunication;
	Core::Event mEvents;
	ServiceItem mServices[NB_SERVICES];
};
}
}

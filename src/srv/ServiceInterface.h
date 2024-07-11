#pragma once

#include "../cmp/Communication.h"
#include "../cmp/BatteryInterface.h"
#include "../cmp/SensorProximityInterface.h"
#include "../clu/Frame.h"

class ServiceMediatorInterface {
public:
	ServiceMediatorInterface()  = default;
	~ServiceMediatorInterface() = default;
	virtual void SendFrame( Clusters::Frame &message ) const = 0;
	virtual void DisplayBatteryLevel( Component::BatteryInterface::BatteryState state )          = 0;
	virtual void DisplayProximitySensor( Component::SensorProximityInterface::SensorsId sensor ) = 0;
};

class BaseComponent : public Core::CoreInterface {
protected:
	ServiceMediatorInterface *mServiceMediator;

public:
	BaseComponent( ServiceMediatorInterface *ServiceMediatorInterface = nullptr ) : mServiceMediator( ServiceMediatorInterface )
	{
	}

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	void SetComComponent ( ServiceMediatorInterface *ServiceMediatorInterface )
	{
		this->mServiceMediator = ServiceMediatorInterface;
	}
};

class ServiceInterface : public BaseComponent {
public:
	ServiceInterface()  = default;
	~ServiceInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;
};

#pragma once
#include <gmock/gmock.h>
#include "../../../src/srv/ServiceInterface.h"


class MockServiceMediator : public ServiceMediatorInterface {
public:
	MOCK_CONST_METHOD1( SendFrame, void( Clusters::Frame & ) );
	MOCK_METHOD1( DisplayBatteryLevel, void ( Component::BatteryState ) );
	MOCK_METHOD1( DisplayProximitySensor, void ( Component::SensorsId  ) );
};

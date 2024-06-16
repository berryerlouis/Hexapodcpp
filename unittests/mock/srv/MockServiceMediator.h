#pragma once
#include <gmock/gmock.h>
#include "../../../src/srv/ServiceInterface.h"


class MockServiceMediator : public ServiceMediatorInterface {
public:
	MOCK_CONST_METHOD1(SendFrame, void( Cluster::Frame & ) );
};

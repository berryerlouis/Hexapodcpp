#pragma once
#include <gmock/gmock.h>
#include "../../../src/srv/ServiceInterface.h"


class MockService : public ServiceInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );
};

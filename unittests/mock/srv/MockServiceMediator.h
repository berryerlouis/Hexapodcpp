#pragma once
#include <gmock/gmock.h>
#include "../../../src/Service/ServiceInterface.h"

namespace Service {
class MockServiceMediator : public ServiceMediatorInterface {
public:
	MOCK_CONST_METHOD1( SendFrame, void( Frame & ) );
};
}

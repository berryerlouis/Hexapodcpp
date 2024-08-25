#pragma once
#include <gmock/gmock.h>
#include "../../../src/Core/Event/EventMediatorInterface.h"

namespace Core {
class MockEventMediatorInterface : public EventMediatorInterface {
public:
	MOCK_CONST_METHOD1( Notify, void( Event ) );
};
}

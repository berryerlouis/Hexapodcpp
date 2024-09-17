#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Communication/CommunicationInterface.h"

namespace Component {
namespace Communication {
class MockCommunication : public CommunicationInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD1( SendMessage, Core::CoreStatus( Frame & ) );
};
}
}

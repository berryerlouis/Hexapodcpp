#pragma once
#include <gmock/gmock.h>
#include "../../../src/Driver/Socket/SocketInterface.h"

namespace Driver
{
	namespace Socket
	{
		class MockSocket : public SocketInterface {
		public:
			MOCK_METHOD0(Initialize, Core::Status( void ));
			MOCK_METHOD1(Update, void( const uint64_t ));

			MOCK_METHOD2(Send, void( const char *, const size_t ));
			MOCK_METHOD1(Send, void( const uint8_t ));
			MOCK_METHOD0(Read, uint8_t( void ));
			MOCK_METHOD0(DataAvailable, uint8_t( void ));
		};
	}
}

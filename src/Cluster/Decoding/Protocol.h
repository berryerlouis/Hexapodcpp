#pragma once

#include "../Frame/Frame.h"
#include <string.h>

namespace Cluster {
namespace Decoding {

class Protocol
{
public:
	enum ProtocolStatus
	{
		NO_ERROR,
		ERROR_LENGHT,
		ERROR_CHAR_INVALID,
		ERROR_SIZE_PARAMS,
		ERROR_NULL_BUFFER,
	};

	Protocol( void );
	~Protocol() = default;

	static ProtocolStatus Decode( const char *frameBuffer, Frame &frame );
	static uint8_t Encode( Frame &response, const uint8_t *buffer );

private:
	static uint8_t ConvertHexCharToInt( uint8_t byte );
};
}
}

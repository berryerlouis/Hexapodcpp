#pragma once

#include "string.h"
#include "../drv/UartInterface.h"

#ifdef DEBUG
#define INIT_LOG( uart )    Core::Logger = Core::Log::GetInstance( uart );
#define LOG( value )        Core::Logger->Write( value )
#else
#define INIT_LOG( uart )
#define LOG( value )
#endif
namespace Core {
class Log {
private:
	Driver::UartInterface &mUart;

	Log( Driver::UartInterface &uart ) : mUart( uart )
	{
	}

	Log( Log & ) = delete;

public:
	static Log *GetInstance( Driver::UartInterface &uart );

	void Write( const char *data );
	void Write( const uint8_t data );
	void Write( const uint16_t data );
	void Write( const uint32_t data );
	void Write( const uint64_t data );
	void Write( const int8_t data );
	void Write( const int16_t data );
	void Write( const int32_t data );
	void Write( const int64_t data );
};
extern Log *Logger;
}

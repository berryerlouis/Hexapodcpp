#include "Log.h"
#include <stdlib.h>


namespace Core {
Log *Log::GetInstance ( Driver::UartInterface &uart )
{
	static Log instance( uart );
	return ( &instance );
}

void Log::Write ( const char *data )
{
	mUart.Send( data, strlen( data ) );
	mUart.Send( '\n' );
}

void Log::Write ( const uint64_t data )
{
	char string[10U];
	Log::Write( ltoa( data, string, 10 ) );
}

void Log::Write ( const uint32_t data )
{
	char string[10U];
	Log::Write( ltoa( data, string, 10 ) );
}

void Log::Write ( const uint16_t data )
{
	char string[10U];
	Log::Write( itoa( data, string, 10 ) );
}

void Log::Write ( const uint8_t data )
{
	char string[10U];
	Log::Write( itoa( data, string, 10 ) );
}

void Log::Write ( const int64_t data )
{
	char string[10U];
	Log::Write( ltoa( data, string, 10 ) );
}

void Log::Write ( const int32_t data )
{
	char string[10U];
	Log::Write( ltoa( data, string, 10 ) );
}

void Log::Write ( const int16_t data )
{
	char string[10U];
	Log::Write( itoa( data, string, 10 ) );
}

void Log::Write ( const int8_t data )
{
	char string[10U];
	Log::Write( itoa( data, string, 10 ) );
}

Log *Logger;
}

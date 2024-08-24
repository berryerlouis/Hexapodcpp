#include "Logger.h"
#include <stdlib.h>

namespace Misc {
namespace Logger {
Logger *Logger::GetInstance ( Driver::Uart::UartInterface &uart )
{
	static Logger instance( uart );
	return ( &instance );
}

void Logger::Write ( const char *data )
{
	mUart.Send( data, strlen( data ) );
}

void Logger::Write ( const uint64_t data )
{
	char string[10U];
	Logger::Write( ltoa( data, string, 10 ) );
}

void Logger::Write ( const uint32_t data )
{
	char string[10U];
	Logger::Write( ltoa( data, string, 10 ) );
}

void Logger::Write ( const uint16_t data )
{
	char string[10U];
	Logger::Write( itoa( data, string, 10 ) );
}

void Logger::Write ( const uint8_t data )
{
	char string[10U];
	Logger::Write( itoa( data, string, 10 ) );
}

void Logger::Write ( const int64_t data )
{
	char string[10U];
	Logger::Write( ltoa( data, string, 10 ) );
}

void Logger::Write ( const int32_t data )
{
	char string[10U];
	Logger::Write( ltoa( data, string, 10 ) );
}

void Logger::Write ( const int16_t data )
{
	char string[10U];
	Logger::Write( itoa( data, string, 10 ) );
}

void Logger::Write ( const int8_t data )
{
	char string[10U];
	Logger::Write( itoa( data, string, 10 ) );
}

Logger *Log;
}
}

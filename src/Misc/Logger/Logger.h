#pragma once

#include "string.h"
#include "../../Driver/Uart/UartInterface.h"

#ifdef GTEST

#include <stdio.h>
#include <iostream>

#endif

#ifdef DEBUG
#ifndef GTEST
#define INIT_LOGGER(uart)    Misc::Logger::Log = Misc::Logger::Logger::GetInstance( uart );
#define LOG(value)           Misc::Logger::Log->Write( value )
#else
#define INIT_LOGGER(uart)
#define LOG(value)           std::cout << value << std::endl;
/*
 * printf (const char *__restrict __fmt, ...)
{
  return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
}
 */
#endif
#else
#define INIT_LOGGER( uart )
#define LOG( value )
#endif

namespace Misc
{
    namespace Logger
    {
        class Logger {
        public:
            static Logger *GetInstance(Driver::Uart::UartInterface &uart);

            void Write(const char *data) const;

        private:
            Driver::Uart::UartInterface &mUart;

            Logger(Driver::Uart::UartInterface &uart) : mUart(uart) {
            }

            Logger(Logger &) = delete;
        };

        extern Logger *Log;
    }
}

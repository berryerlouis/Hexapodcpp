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

            void Write(const char *data);

            void Write(const uint8_t data);

            void Write(const uint16_t data);

            void Write(const uint32_t data);

            void Write(const uint64_t data);

            void Write(const int8_t data);

            void Write(const int16_t data);

            void Write(const int32_t data);

            void Write(const int64_t data);

        private:
            Driver::Uart::UartInterface &mUart;

            Logger(Driver::Uart::UartInterface &uart) : mUart(uart) {
            }

            Logger(Logger &) = delete;
        };

        extern Logger *Log;
    }
}

#include "Logger.h"
#include <string.h>
#include <stdio.h>

namespace Misc
{
    namespace Logger
    {
        Logger *Logger::GetInstance(Driver::Uart::UartInterface &uart) {
            static Logger instance(uart);
            return (&instance);
        }

        void Logger::Write(const char *data) {
            mUart.Send(data, strlen(data));
        }

        void Logger::Write(const uint64_t data) {
            char string[10U];
#ifndef GTEST
            Logger::Write(sprintf(string, "%lld", data));
#else
    Logger::Write(sprintf(string, "%ld", data));
#endif
        }

        void Logger::Write(const uint32_t data) {
            char string[10U];
#ifndef GTEST
            Logger::Write(sprintf(string, "%ld", data));
#else
    Logger::Write(sprintf(string, "%d", data));
#endif
        }

        void Logger::Write(const uint16_t data) {
            char string[10U];
            Logger::Write(sprintf(string, "%d", data));
        }

        void Logger::Write(const uint8_t data) {
            char string[10U];
            Logger::Write(sprintf(string, "%d", data));
        }

        void Logger::Write(const int64_t data) {
            char string[10U];
#ifndef GTEST
            Logger::Write(sprintf(string, "%lld", data));
#else
    Logger::Write(sprintf(string, "%ld", data));
#endif
        }

        void Logger::Write(const int32_t data) {
            char string[10U];
#ifndef GTEST
            Logger::Write(sprintf(string, "%ld", data));
#else
    Logger::Write(sprintf(string, "%d", data));
#endif
        }

        void Logger::Write(const int16_t data) {
            char string[10U];
#ifndef GTEST
            Logger::Write(sprintf(string, "%d", (int16_t) data));
#else
    Logger::Write(sprintf(string, "%d", data));
#endif
        }

        void Logger::Write(const int8_t data) {
            char string[10U];
            Logger::Write(sprintf(string, "%d", data));
        }

        Logger *Log;
    }
}

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

        void Logger::Write(const char *data) const {
            mUart.Send(data, strlen(data));
        }

        Logger *Log;
    }
}

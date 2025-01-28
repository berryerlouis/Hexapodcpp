#include "Logger.h"
#include <string.h>
#include <stdio.h>

namespace Misc
{
    namespace Logger
    {
        Logger *Logger::GetInstance(Driver::Socket::SocketInterface &socket) {
            static Logger instance(socket);
            return (&instance);
        }

        void Logger::Write(const char *data) const {
            mSocket.Send(data, strlen(data));
        }

        Logger *Log;
    }
}

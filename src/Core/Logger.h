#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdarg>

namespace Core
{
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        NONE,
        UNKNOWN
    };

    // ANSI color codes
#define LOG_COLOR_RESET "\033[0m"
#define LOG_COLOR_RED "\033[31m"
#define LOG_COLOR_YELLOW "\033[33m"
#define LOG_COLOR_GREEN "\033[32m"
#define LOG_COLOR_CYAN "\033[36m"
#define LOG_COLOR_WHITE "\033[37m"


    class Logger {

    public:
        static void Log(const LogLevel level, const char *format, ...);

        static LogLevel GetLogLevel();

        static void SetLogLevel(const LogLevel level);

        static LogLevel StringToLevel(const char *level);

    private:
        static LogLevel currentLogLevel;

        static std::string FormatString(const char *format, va_list args);

        static std::string LevelToString(const LogLevel level);

        static std::string GetColor(const LogLevel level);

        static std::string CurrentTime();
    };

#define LOG(level, ...) Core::Logger::Log(level, __VA_ARGS__)
#define LOG_DEBUG(...) Core::Logger::Log(Core::LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) Core::Logger::Log(Core::LogLevel::INFO, __VA_ARGS__)
#define LOG_WARNING(...) Core::Logger::Log(Core::LogLevel::WARNING, __VA_ARGS__)
#define LOG_ERROR(...) Core::Logger::Log(Core::LogLevel::ERROR, __VA_ARGS__)

#define LOG_GENERIC(level, component, ...) LOG_##level("[" #component "] " __VA_ARGS__)
}

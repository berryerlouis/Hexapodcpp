#pragma once

#include <cstdarg>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace Core
{
    enum class LogLevel {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_NONE,
        LOG_UNKNOWN
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
        static void     Log(const LogLevel level, const char *format, ...);

        static LogLevel GetLogLevel();

        static void     SetLogLevel(const LogLevel level);

        static LogLevel StringToLevel(const char *level);

    private:
        static LogLevel    currentLogLevel;

        static std::string FormatString(const char *format, va_list args);

        static std::string LevelToString(const LogLevel level);

        static std::string GetColor(const LogLevel level);

        static std::string CurrentTime();
    };

#define LOG(level, ...) Core::Logger::Log(level, __VA_ARGS__)
#define LOG_DEBUG(...) Core::Logger::Log(Core::LogLevel::LOG_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) Core::Logger::Log(Core::LogLevel::LOG_INFO, __VA_ARGS__)
#define LOG_WARNING(...) \
    Core::Logger::Log(Core::LogLevel::LOG_WARNING, __VA_ARGS__)
#define LOG_ERROR(...) Core::Logger::Log(Core::LogLevel::LOG_ERROR, __VA_ARGS__)

#define LOG_GENERIC(level, component, ...) \
    LOG_##level("[" #component "] " __VA_ARGS__)
} // namespace Core

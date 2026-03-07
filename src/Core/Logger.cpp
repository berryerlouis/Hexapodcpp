#include "Logger.h"

#include <chrono>
#include <cstring>

namespace Core
{
    LogLevel Logger::currentLogLevel = LogLevel::LOG_DEBUG;

    void     Logger::Log(const LogLevel level, const char *format, ...) {
        if (GetLogLevel() < LogLevel::LOG_NONE && level < LogLevel::LOG_NONE &&
            level >= GetLogLevel()) {
            va_list args;
            va_start(args, format);
            const std::string formatted = FormatString(format, args);
            va_end(args);

            const std::string color = GetColor(level);
            const std::string levelStr = LevelToString(level);

            std::cout << "[" << CurrentTime() << "] " << color << "[" << levelStr << "] "
                      << formatted << LOG_COLOR_RESET << "\n";
        }
    }

    LogLevel Logger::GetLogLevel() {
        return currentLogLevel;
    }

    void Logger::SetLogLevel(const LogLevel level) {
        currentLogLevel = level;
    }

    LogLevel Logger::StringToLevel(const char *level) {
        if (0U == strcmp(level, "DEBUG")) {
            return LogLevel::LOG_DEBUG;
        }
        if (0U == strcmp(level, "INFO")) {
            return LogLevel::LOG_INFO;
        }
        if (0U == strcmp(level, "WARNING")) {
            return LogLevel::LOG_WARNING;
        }
        if (0U == strcmp(level, "ERROR")) {
            return LogLevel::LOG_ERROR;
        }
        if (0U == strcmp(level, "NONE")) {
            return LogLevel::LOG_NONE;
        }
        return LogLevel::LOG_UNKNOWN;
    }

    std::string Logger::FormatString(const char *format, va_list args) {
        char buffer[1024U];
        vsnprintf(buffer, sizeof(buffer), format, args);
        return std::string(buffer);
    }

    std::string Logger::LevelToString(const LogLevel level) {
        switch (level) {
            case LogLevel::LOG_DEBUG:
                return "DEBUG";
            case LogLevel::LOG_INFO:
                return "INFO";
            case LogLevel::LOG_WARNING:
                return "WARNING";
            case LogLevel::LOG_ERROR:
                return "ERROR";
            case LogLevel::LOG_NONE:
                return "NONE";
            default:
                return "UNKNOWN";
        }
    }

    std::string Logger::GetColor(const LogLevel level) {
        switch (level) {
            case LogLevel::LOG_DEBUG:
                return LOG_COLOR_CYAN;
            case LogLevel::LOG_INFO:
                return LOG_COLOR_GREEN;
            case LogLevel::LOG_WARNING:
                return LOG_COLOR_YELLOW;
            case LogLevel::LOG_ERROR:
                return LOG_COLOR_RED;
            default:
                return LOG_COLOR_WHITE;
        }
    }

    std::string Logger::CurrentTime() {
        // Get current time_point
        const auto now = std::chrono::system_clock::now();
        const auto now_time_t = std::chrono::system_clock::to_time_t(now);
        const auto now_ms =
                std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) %
                1000;

        // Convert to local time
        const std::tm *local_tm = std::localtime(&now_time_t);

        // Format time with milliseconds
        std::ostringstream oss;
        oss << std::put_time(local_tm, "%Y-%m-%d %H:%M:%S");
        oss << '.' << std::setfill('0') << std::setw(3) << now_ms.count();

        return oss.str();
    }
} // namespace Core

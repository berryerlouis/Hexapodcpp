#include "App/App.h"
#include "Core/Logger.h"
#include "Core/Version.h"

int main(const int argc,
         char    **argv) {
    setvbuf(stdout, nullptr, _IONBF, 0U);

    LOG_INFO("Hexapod started.");
    LOG_INFO("Hexapod version: %d.%d", VERSION_MAJOR, VERSION_MINOR);
    Core::Logger::SetLogLevel(Core::LogLevel::DEBUG);
    if (argc > 1) {
        const Core::LogLevel level = Core::Logger::StringToLevel(argv[1U]);
        if (level != Core::LogLevel::UNKNOWN) {
            Core::Logger::SetLogLevel(level);
        }
    }

    App::App robot;
    // hexapod initialization
    if (robot.Initialize() == Core::Status::CORE_OK) {
        while (true) {
            // hexapod loop update
            robot.Update();
        }
    }
    return -1;
}

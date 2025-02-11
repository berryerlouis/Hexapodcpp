#include <unistd.h>
#include "Builder/App.h"

using namespace Builder;

Builder::App robot;

int main(void) {
    // hexapod initialization
    if (robot.Initialize() == Core::Status::CORE_OK) {
        while (true) {
            // hexapod loop update
            robot.Update();
            usleep(1000);
        }
    }
    return -1;
}


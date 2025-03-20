#include "Builder/App.h"

using namespace Builder;

Builder::App robot;

int main(void) {
    // hexapod initialization
    if (robot.Initialize() == Core::Status::CORE_OK) {
        while (true) {
            // hexapod loop update
            robot.Update();
        }
    }
    return -1;
}


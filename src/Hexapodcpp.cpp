// ReSharper disable CppDFAEndlessLoop
#ifdef AVR
#include <avr/interrupt.h>
#include <avr/wdt.h>
#endif
#include "Builder/App.h"

using namespace Builder;

Builder::App robot;


int main(void) {
#ifdef AVR
    cli();
    wdt_disable();
    sei();
#endif

    // hexapod initialization
    if (robot.Initialize() == Core::Status::CORE_OK) {
        // enable ITs
#ifdef AVR
        wdt_enable(WDTO_15MS);
#endif
        while (true) {
#ifdef AVR
            wdt_reset();
#endif
            // hexapod loop update
            robot.Update();
        }
    }
    return -1;
}


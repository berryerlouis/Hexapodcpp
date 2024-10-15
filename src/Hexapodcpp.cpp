#ifndef GTEST
#include <avr/interrupt.h>
#include <avr/wdt.h>
#endif
#include "Builder/App.h"

using namespace Builder;

Builder::App robot;

int main(void) {
#ifndef GTEST
    cli();
    wdt_disable();
#endif
    //hexapod initialization
    if (robot.Initialize() == Core::CoreStatus::CORE_OK) {
        //enable ITs
#ifndef GTEST
        sei();
        wdt_enable(WDTO_15MS);
#endif
        while (true) {
#ifndef GTEST
            wdt_reset();
#endif
            //hexapod loop update
            robot.Update(0U);
        }
    }

    return (-1);
}

/*
 * I2C address 0x29  !
 * I2C address 0x3C  !
 * I2C address 0x40  !
 * I2C address 0x41  !
 * I2C address 0x69  !
 * I2C address 0x70  !
 * I2C address 0x77  !
 */

#include <Arduino.h>
#include "src/app/Builder.h"

Builder app;

void setup()
{
  app.Initialize();
}

void loop()
{
  app.Update();
}



















//arriere "<05000e000000000000188016800000f401>"
// x -10 <05000e0A8000000000000000000000e803>
// x +10 <05000e0A0000000000000000000000e803>
// y -10 <05000e00000F800000000000000000e803>
// y +10 <05000e00000F000000000000000000e803>
// z -10 <05000e000000000A80000000000000e803>
// z +10 <05000e 0000 0000 0A00 000000000000>

//<05000C000000000000000000000000><030000><05000c000000001000000000000000><030000>

/*
14:09:09.392 -> I2C device found at address 0x29  !
14:09:09.438 -> I2C device found at address 0x3C  !
14:09:09.438 -> I2C device found at address 0x40  !
14:09:09.438 -> I2C device found at address 0x41  !
14:09:09.438 -> I2C device found at address 0x69  !
14:09:09.438 -> I2C device found at address 0x70  !
14:09:09.438 -> I2C device found at address 0x77  !
*/

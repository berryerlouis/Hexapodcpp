#include <avr/interrupt.h>
#include "app/App.h"

using namespace app;

App robot;

int main (void)
{
	//hexapod initialization
	if (robot.Initialize() )
	{
		//enable ITs
		sei();

		while (true)
		{
			//hexapod loop update
			robot.Update();
		}
	}

	while (true)
	{
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

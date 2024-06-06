#define _AVR_IOM1284P_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include "app/App.h"

App app;

int main (void)
{
	//hexapod initialization
	app.Initialize();

	//enable ITs
	sei();

	while (true)
	{
		//hexapod loop update
		app.Update();
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

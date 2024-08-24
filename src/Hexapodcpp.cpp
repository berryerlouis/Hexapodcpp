#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "App/App.h"

using namespace App;

App::App robot;

int main ( void )
{
	//hexapod initialization
	if ( robot.Initialize() )
	{
		//enable ITs
		sei();

		wdt_enable( WDTO_15MS );
		while ( true )
		{
			wdt_reset();
			//hexapod loop update
			robot.Update( 0UL );
		}
	}

	return ( -1 );
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
